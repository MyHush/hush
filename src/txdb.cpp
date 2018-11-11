// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2018      The Hush developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "txdb.h"

#include "chainparams.h"
#include "hash.h"
#include "main.h"
#include "pow.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/thread.hpp>

using namespace std;

static const char DB_ANCHOR = 'A';
static const char DB_NULLIFIER = 's';
static const char DB_COINS = 'c';
static const char DB_BLOCK_FILES = 'f';
static const char DB_TXINDEX = 't';

static const char DB_ADDRESSINDEX = 'd';
static const char DB_ADDRESSUNSPENTINDEX = 'u';
static const char DB_TIMESTAMPINDEX = 'S';
static const char DB_BLOCKHASHINDEX = 'z';
static const char DB_SPENTINDEX = 'p';
static const char DB_BLOCK_INDEX = 'b';

static const char DB_BEST_BLOCK = 'B';
static const char DB_BEST_ANCHOR = 'a';
static const char DB_FLAG = 'F';
static const char DB_REINDEX_FLAG = 'R';
static const char DB_LAST_BLOCK = 'l';


void static BatchWriteAnchor(CLevelDBBatch &batch,
                             const uint256 &croot,
                             const ZCIncrementalMerkleTree &tree,
                             const bool &entered)
{
    if (!entered)
        batch.Erase(make_pair(DB_ANCHOR, croot));
    else {
        batch.Write(make_pair(DB_ANCHOR, croot), tree);
    }
}

void static BatchWriteNullifier(CLevelDBBatch &batch, const uint256 &nf, const bool &entered) {
    if (!entered)
        batch.Erase(make_pair(DB_NULLIFIER, nf));
    else
        batch.Write(make_pair(DB_NULLIFIER, nf), true);
}

void static BatchWriteCoins(CLevelDBBatch &batch, const uint256 &hash, const CCoins &coins) {
    if (coins.IsPruned())
        batch.Erase(make_pair(DB_COINS, hash));
    else
        batch.Write(make_pair(DB_COINS, hash), coins);
}

void static BatchWriteHashBestChain(CLevelDBBatch &batch, const uint256 &hash) {
    batch.Write(DB_BEST_BLOCK, hash);
}

void static BatchWriteHashBestAnchor(CLevelDBBatch &batch, const uint256 &hash) {
    batch.Write(DB_BEST_ANCHOR, hash);
}

CCoinsViewDB::CCoinsViewDB(std::string dbName, size_t nCacheSize, bool fMemory, bool fWipe) : db(GetDataDir() / dbName, nCacheSize, fMemory, fWipe, false, 64) {
}

CCoinsViewDB::CCoinsViewDB(size_t nCacheSize, bool fMemory, bool fWipe) : db(GetDataDir() / "chainstate", nCacheSize, fMemory, fWipe, false, 64) {
}


bool CCoinsViewDB::GetAnchorAt(const uint256 &rt, ZCIncrementalMerkleTree &tree) const {
    if (rt == ZCIncrementalMerkleTree::empty_root()) {
        ZCIncrementalMerkleTree new_tree;
        tree = new_tree;
        return true;
    }

    bool read = db.Read(make_pair(DB_ANCHOR, rt), tree);

    return read;
}

bool CCoinsViewDB::GetNullifier(const uint256 &nf) const {
    bool spent = false;
    bool read = db.Read(make_pair(DB_NULLIFIER, nf), spent);

    return read;
}

bool CCoinsViewDB::GetCoins(const uint256 &txid, CCoins &coins) const {
    return db.Read(make_pair(DB_COINS, txid), coins);
}

bool CCoinsViewDB::HaveCoins(const uint256 &txid) const {
    return db.Exists(make_pair(DB_COINS, txid));
}

uint256 CCoinsViewDB::GetBestBlock() const {
    uint256 hashBestChain;
    if (!db.Read(DB_BEST_BLOCK, hashBestChain))
        return uint256();
    return hashBestChain;
}

uint256 CCoinsViewDB::GetBestAnchor() const {
    uint256 hashBestAnchor;
    if (!db.Read(DB_BEST_ANCHOR, hashBestAnchor))
        return ZCIncrementalMerkleTree::empty_root();
    return hashBestAnchor;
}

bool CCoinsViewDB::BatchWrite(CCoinsMap &mapCoins,
                              const uint256 &hashBlock,
                              const uint256 &hashAnchor,
                              CAnchorsMap &mapAnchors,
                              CNullifiersMap &mapNullifiers) {
    CLevelDBBatch batch;
    size_t count = 0;
    size_t changed = 0;
    for (CCoinsMap::iterator it = mapCoins.begin(); it != mapCoins.end();) {
        if (it->second.flags & CCoinsCacheEntry::DIRTY) {
            BatchWriteCoins(batch, it->first, it->second.coins);
            changed++;
        }
        count++;
        CCoinsMap::iterator itOld = it++;
        mapCoins.erase(itOld);
    }

    for (CAnchorsMap::iterator it = mapAnchors.begin(); it != mapAnchors.end();) {
        if (it->second.flags & CAnchorsCacheEntry::DIRTY) {
            BatchWriteAnchor(batch, it->first, it->second.tree, it->second.entered);
            // TODO: changed++?
        }
        CAnchorsMap::iterator itOld = it++;
        mapAnchors.erase(itOld);
    }

    for (CNullifiersMap::iterator it = mapNullifiers.begin(); it != mapNullifiers.end();) {
        if (it->second.flags & CNullifiersCacheEntry::DIRTY) {
            BatchWriteNullifier(batch, it->first, it->second.entered);
            // TODO: changed++?
        }
        CNullifiersMap::iterator itOld = it++;
        mapNullifiers.erase(itOld);
    }

    if (!hashBlock.IsNull())
        BatchWriteHashBestChain(batch, hashBlock);
    if (!hashAnchor.IsNull())
        BatchWriteHashBestAnchor(batch, hashAnchor);

    LogPrint("coindb", "Committing %u changed transactions (out of %u) to coin database...\n", (unsigned int)changed, (unsigned int)count);
    return db.WriteBatch(batch);
}

CBlockTreeDB::CBlockTreeDB(size_t nCacheSize, bool fMemory, bool fWipe, bool compression, int maxOpenFiles) : CLevelDBWrapper(GetDataDir() / "blocks" / "index", nCacheSize, fMemory, fWipe, compression, maxOpenFiles) {
}

bool CBlockTreeDB::ReadBlockFileInfo(int nFile, CBlockFileInfo &info) {
    return Read(make_pair(DB_BLOCK_FILES, nFile), info);
}

bool CBlockTreeDB::WriteReindexing(bool fReindexing) {
    if (fReindexing)
        return Write(DB_REINDEX_FLAG, '1');
    else
        return Erase(DB_REINDEX_FLAG);
}

bool CBlockTreeDB::ReadReindexing(bool &fReindexing) {
    fReindexing = Exists(DB_REINDEX_FLAG);
    return true;
}

bool CBlockTreeDB::ReadLastBlockFile(int &nFile) {
    return Read(DB_LAST_BLOCK, nFile);
}

bool CCoinsViewDB::GetStats(CCoinsStats &stats) const {
    /* It seems that there are no "const iterators" for LevelDB.  Since we
       only need read operations on it, use a const-cast to get around
       that restriction.  */
    boost::scoped_ptr<leveldb::Iterator> pcursor(const_cast<CLevelDBWrapper*>(&db)->NewIterator());
    pcursor->SeekToFirst();

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    stats.hashBlock = GetBestBlock();
    ss << stats.hashBlock;
    CAmount nTotalAmount = 0;
    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            leveldb::Slice slKey = pcursor->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            char chType;
            ssKey >> chType;
            if (chType == DB_COINS) {
                leveldb::Slice slValue = pcursor->value();
                CDataStream ssValue(slValue.data(), slValue.data()+slValue.size(), SER_DISK, CLIENT_VERSION);
                CCoins coins;
                ssValue >> coins;
                uint256 txhash;
                ssKey >> txhash;
                ss << txhash;
                ss << VARINT(coins.nVersion);
                ss << (coins.fCoinBase ? 'c' : 'n');
                ss << VARINT(coins.nHeight);
                stats.nTransactions++;
                for (unsigned int i=0; i<coins.vout.size(); i++) {
                    const CTxOut &out = coins.vout[i];
                    if (!out.IsNull()) {
                        stats.nTransactionOutputs++;
                        ss << VARINT(i+1);
                        ss << out;
                        nTotalAmount += out.nValue;
                    }
                }
                stats.nSerializedSize += 32 + slValue.size();
                ss << VARINT(0);
            }
            pcursor->Next();
        } catch (const std::exception& e) {
            return error("%s: Deserialize or I/O error - %s", __func__, e.what());
        }
    }
    {
        LOCK(cs_main);
        stats.nHeight = mapBlockIndex.find(stats.hashBlock)->second->nHeight;
    }
    stats.hashSerialized = ss.GetHash();
    stats.nTotalAmount = nTotalAmount;
    return true;
}

bool CBlockTreeDB::WriteBatchSync(const std::vector<std::pair<int, const CBlockFileInfo*> >& fileInfo, int nLastFile, const std::vector<const CBlockIndex*>& blockinfo) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<int, const CBlockFileInfo*> >::const_iterator it=fileInfo.begin(); it != fileInfo.end(); it++) {
        batch.Write(make_pair(DB_BLOCK_FILES, it->first), *it->second);
    }
    batch.Write(DB_LAST_BLOCK, nLastFile);
    for (std::vector<const CBlockIndex*>::const_iterator it=blockinfo.begin(); it != blockinfo.end(); it++) {
        batch.Write(make_pair(DB_BLOCK_INDEX, (*it)->GetBlockHash()), CDiskBlockIndex(*it));
    }
    return WriteBatch(batch, true);
}

bool CBlockTreeDB::ReadTxIndex(const uint256 &txid, CDiskTxPos &pos) {
    return Read(make_pair(DB_TXINDEX, txid), pos);
}

bool CBlockTreeDB::WriteTxIndex(const std::vector<std::pair<uint256, CDiskTxPos> >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<uint256,CDiskTxPos> >::const_iterator it=vect.begin(); it!=vect.end(); it++)
        batch.Write(make_pair(DB_TXINDEX, it->first), it->second);
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadSpentIndex(CSpentIndexKey &key, CSpentIndexValue &value) {
    return Read(make_pair(DB_SPENTINDEX, key), value);
}

bool CBlockTreeDB::UpdateSpentIndex(const std::vector<std::pair<CSpentIndexKey, CSpentIndexValue> >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<CSpentIndexKey,CSpentIndexValue> >::const_iterator it=vect.begin(); it!=vect.end(); it++) {
        if (it->second.IsNull()) {
            batch.Erase(make_pair(DB_SPENTINDEX, it->first));
        } else {
            batch.Write(make_pair(DB_SPENTINDEX, it->first), it->second);
        }
    }
    return WriteBatch(batch);
}

bool CBlockTreeDB::UpdateAddressUnspentIndex(const std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue > >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue> >::const_iterator it=vect.begin(); it!=vect.end(); it++) {
        if (it->second.IsNull()) {
            batch.Erase(make_pair(DB_ADDRESSUNSPENTINDEX, it->first));
        } else {
            batch.Write(make_pair(DB_ADDRESSUNSPENTINDEX, it->first), it->second);
        }
    }
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadAddressUnspentIndex(uint160 addressHash, int type,
                                           std::vector<std::pair<CAddressUnspentKey, CAddressUnspentValue> > &unspentOutputs) {

    boost::scoped_ptr<leveldb::Iterator> pcursor(NewIterator());

    CDataStream ssKeySet(SER_DISK, CLIENT_VERSION);
    ssKeySet << make_pair(DB_ADDRESSUNSPENTINDEX, CAddressIndexIteratorKey(type, addressHash));
    pcursor->Seek(ssKeySet.str());

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            leveldb::Slice slKey = pcursor->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            char chType;
            CAddressUnspentKey indexKey;
            ssKey >> chType;
            ssKey >> indexKey;
            if (chType == DB_ADDRESSUNSPENTINDEX && indexKey.hashBytes == addressHash) {
                try {
                    leveldb::Slice slValue = pcursor->value();
                    CDataStream ssValue(slValue.data(), slValue.data()+slValue.size(), SER_DISK, CLIENT_VERSION);
                    CAddressUnspentValue nValue;
                    ssValue >> nValue;
                    unspentOutputs.push_back(make_pair(indexKey, nValue));
                    pcursor->Next();
                } catch (const std::exception& e) {
                    return error("failed to get address unspent value");
                }
            } else {
                break;
            }
        } catch (const std::exception& e) {
            break;
        }
    }

    return true;
}

bool CBlockTreeDB::WriteAddressIndex(const std::vector<std::pair<CAddressIndexKey, CAmount > >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressIndexKey, CAmount> >::const_iterator it=vect.begin(); it!=vect.end(); it++)
        batch.Write(make_pair(DB_ADDRESSINDEX, it->first), it->second);
    return WriteBatch(batch);
}

bool CBlockTreeDB::EraseAddressIndex(const std::vector<std::pair<CAddressIndexKey, CAmount > >&vect) {
    CLevelDBBatch batch;
    for (std::vector<std::pair<CAddressIndexKey, CAmount> >::const_iterator it=vect.begin(); it!=vect.end(); it++)
        batch.Erase(make_pair(DB_ADDRESSINDEX, it->first));
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadAddressIndex(uint160 addressHash, int type,
                                    std::vector<std::pair<CAddressIndexKey, CAmount> > &addressIndex,
                                    int start, int end) {

    boost::scoped_ptr<leveldb::Iterator> pcursor(NewIterator());

    CDataStream ssKeySet(SER_DISK, CLIENT_VERSION);
    if (start > 0 && end > 0) {
        ssKeySet << make_pair(DB_ADDRESSINDEX, CAddressIndexIteratorHeightKey(type, addressHash, start));
    } else {
        ssKeySet << make_pair(DB_ADDRESSINDEX, CAddressIndexIteratorKey(type, addressHash));
    }
    pcursor->Seek(ssKeySet.str());

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            leveldb::Slice slKey = pcursor->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            char chType;
            CAddressIndexKey indexKey;
            ssKey >> chType;
            ssKey >> indexKey;
            if (chType == DB_ADDRESSINDEX && indexKey.hashBytes == addressHash) {
                if (end > 0 && indexKey.blockHeight > end) {
                    break;
                }
                try {
                    leveldb::Slice slValue = pcursor->value();
                    CDataStream ssValue(slValue.data(), slValue.data()+slValue.size(), SER_DISK, CLIENT_VERSION);
                    CAmount nValue;
                    ssValue >> nValue;

                    addressIndex.push_back(make_pair(indexKey, nValue));
                    pcursor->Next();
                } catch (const std::exception& e) {
                    return error("failed to get address index value");
                }
            } else {
                break;
            }
        } catch (const std::exception& e) {
            break;
        }
    }

    return true;
}

bool getAddressFromIndex(const int &type, const uint160 &hash, std::string &address);

extern UniValue CBlockTreeDB::Snapshot(int top)
{
    char chType; int64_t total = 0; int64_t totalAddresses = 0; std::string address;
    int64_t utxos = 0; int64_t ignoredAddresses;
    boost::scoped_ptr<leveldb::Iterator> iter(NewIterator());
    std::map <std::string, CAmount> addressAmounts;
    std::vector <std::pair<CAmount, std::string>> vaddr;
    UniValue result(UniValue::VOBJ);
    result.push_back(Pair("start_time", (int) time(NULL)));

    // TODO: do we have addresses we want to ignore?
    std::map <std::string,int> ignoredMap = {
        {"RReUxSs5hGE39ELU23DfydX8riUuzdrHAE", 1},
        {"RD6GgnrMpPaTSMn8vai6yiGA7mN4QGPVMY", 1} //Burnaddress for null privkey
    };

    int64_t startingHeight = chainActive.Height();
    //fprintf(stderr, "Starting snapshot at height %lli\n", startingHeight);
    for (iter->SeekToLast(); iter->Valid(); iter->Prev())
    {
        boost::this_thread::interruption_point();
        try
        {
            leveldb::Slice slKey = iter->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            CAddressIndexIteratorKey indexKey;

            ssKey >> chType;
            ssKey >> indexKey;

            //fprintf(stderr, "chType=%d\n", chType);
            if (chType == DB_ADDRESSUNSPENTINDEX)
            {
                try {
                    leveldb::Slice slValue = iter->value();
                    CDataStream ssValue(slValue.data(), slValue.data()+slValue.size(), SER_DISK, CLIENT_VERSION);
                    CAmount nValue;
                    ssValue >> nValue;

                    getAddressFromIndex(indexKey.type, indexKey.hashBytes, address);

                    std::map <std::string, int>::iterator ignored = ignoredMap.find(address);
                    if (ignored != ignoredMap.end()) {
                        fprintf(stderr,"ignoring %s\n", address.c_str());
                        ignoredAddresses++;
                        continue;
                    }

                    std::map <std::string, CAmount>::iterator pos = addressAmounts.find(address);
                    if (pos == addressAmounts.end()) {
                        // insert new address + utxo amount
                        //fprintf(stderr, "inserting new address %s with amount %li\n", address.c_str(), nValue);
                        addressAmounts[address] = nValue;
                        totalAddresses++;
                    } else {
                        // update unspent tally for this address
                        //fprintf(stderr, "updating address %s with new utxo amount %li\n", address.c_str(), nValue);
                        addressAmounts[address] += nValue;
                    }
                    //fprintf(stderr,"{\"%s\", %.8f},\n",address.c_str(),(double)nValue/COIN);
                    // total += nValue;
                    utxos++;
                } catch (const std::exception& e) {
                    fprintf(stderr, "DONE %s: LevelDB addressindex exception! - %s\n", __func__, e.what());
                    break;
                }
            }
        } catch (const std::exception& e) {
            fprintf(stderr, "DONE reading index entries\n");
            break;
        }
    }

    UniValue addresses(UniValue::VARR);
    //fprintf(stderr, "total=%f, totalAddresses=%li, utxos=%li, ignored=%li\n", (double) total / COIN, totalAddresses, utxos, ignoredAddresses);

    for (std::pair<std::string, CAmount> element : addressAmounts) {
        vaddr.push_back( make_pair(element.second, element.first) );
    }
    std::sort(vaddr.rbegin(), vaddr.rend());

    UniValue obj(UniValue::VOBJ);
    UniValue addressesSorted(UniValue::VARR);
    int topN = 0;
    for (std::vector<std::pair<CAmount, std::string>>::iterator it = vaddr.begin(); it!=vaddr.end(); ++it) {
        UniValue obj(UniValue::VOBJ);
        obj.push_back( make_pair("addr", it->second.c_str() ) );
        char amount[32];
        sprintf(amount, "%.8f", (double) it->first / COIN);
        obj.push_back( make_pair("amount", amount) );
        total += it->first;
        addressesSorted.push_back(obj);
        topN++;
        // If requested, only show top N addresses in output JSON
        if (top == topN)
            break;
    }

    if (top)
        totalAddresses = top;

    if (totalAddresses > 0) {
        // Array of all addreses with balances
        result.push_back(make_pair("addresses", addressesSorted));
        // Total amount in this snapshot, which is less than circulating supply if top parameter is used
        result.push_back(make_pair("total", (double) total / COIN ));
        // Average amount in each address of this snapshot
        result.push_back(make_pair("average",(double) (total/COIN) / totalAddresses ));
    }
    // Total number of utxos processed in this snaphot
    result.push_back(make_pair("utxos", utxos));
    // Total number of addresses in this snaphot
    result.push_back(make_pair("total_addresses", totalAddresses));
    // Total number of ignored addresses in this snaphot
    result.push_back(make_pair("ignored_addresses", ignoredAddresses));
    // The snapshot began at this block height
    result.push_back(make_pair("start_height", startingHeight));
    // The snapshot finished at this block height
    result.push_back(make_pair("ending_height", chainActive.Height()));
    return(result);
}


bool CBlockTreeDB::WriteTimestampIndex(const CTimestampIndexKey &timestampIndex) {
    CLevelDBBatch batch;
    batch.Write(make_pair(DB_TIMESTAMPINDEX, timestampIndex), 0);
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadTimestampIndex(const unsigned int &high, const unsigned int &low, const bool fActiveOnly, std::vector<std::pair<uint256, unsigned int> > &hashes) {

    boost::scoped_ptr<leveldb::Iterator> pcursor(NewIterator());

    CDataStream ssKeySet(SER_DISK, CLIENT_VERSION);
    ssKeySet << make_pair(DB_TIMESTAMPINDEX, CTimestampIndexIteratorKey(low));
    pcursor->Seek(ssKeySet.str());

    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            leveldb::Slice slKey = pcursor->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            char chType;
            CTimestampIndexKey indexKey;
            ssKey >> chType;
            ssKey >> indexKey;
            if (chType == DB_TIMESTAMPINDEX && indexKey.timestamp < high) {
                if (fActiveOnly) {
                    if (blockOnchainActive(indexKey.blockHash)) {
                        hashes.push_back(std::make_pair(indexKey.blockHash, indexKey.timestamp));
                    }
                } else {
                    hashes.push_back(std::make_pair(indexKey.blockHash, indexKey.timestamp));
                }

                pcursor->Next();
            } else {
                break;
            }
        } catch (const std::exception& e) {
            break;
        }
    }

    return true;
}

bool CBlockTreeDB::WriteTimestampBlockIndex(const CTimestampBlockIndexKey &blockhashIndex, const CTimestampBlockIndexValue &logicalts) {
    CLevelDBBatch batch;
    batch.Write(make_pair(DB_BLOCKHASHINDEX, blockhashIndex), logicalts);
    return WriteBatch(batch);
}

bool CBlockTreeDB::ReadTimestampBlockIndex(const uint256 &hash, unsigned int &ltimestamp) {

    CTimestampBlockIndexValue(lts);
    if (!Read(std::make_pair(DB_BLOCKHASHINDEX, hash), lts))
	return false;

    ltimestamp = lts.ltimestamp;
    return true;
}

bool CBlockTreeDB::WriteFlag(const std::string &name, bool fValue) {
    return Write(std::make_pair(DB_FLAG, name), fValue ? '1' : '0');
}

bool CBlockTreeDB::ReadFlag(const std::string &name, bool &fValue) {
    char ch;
    if (!Read(std::make_pair(DB_FLAG, name), ch))
        return false;
    fValue = ch == '1';
    return true;
}

bool CBlockTreeDB::blockOnchainActive(const uint256 &hash) {
    CBlockIndex* pblockindex = mapBlockIndex[hash];

    if (!chainActive.Contains(pblockindex)) {
	return false;
    }

    return true;
}

bool CBlockTreeDB::LoadBlockIndexGuts()
{
    boost::scoped_ptr<leveldb::Iterator> pcursor(NewIterator());

    CDataStream ssKeySet(SER_DISK, CLIENT_VERSION);
    ssKeySet << make_pair(DB_BLOCK_INDEX, uint256());
    pcursor->Seek(ssKeySet.str());

    // Load mapBlockIndex
    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        try {
            leveldb::Slice slKey = pcursor->key();
            CDataStream ssKey(slKey.data(), slKey.data()+slKey.size(), SER_DISK, CLIENT_VERSION);
            char chType;
            ssKey >> chType;
            if (chType == DB_BLOCK_INDEX) {
                leveldb::Slice slValue = pcursor->value();
                CDataStream ssValue(slValue.data(), slValue.data()+slValue.size(), SER_DISK, CLIENT_VERSION);
                CDiskBlockIndex diskindex;
                ssValue >> diskindex;

                // Construct block index object
                CBlockIndex* pindexNew = InsertBlockIndex(diskindex.GetBlockHash());
                pindexNew->pprev          = InsertBlockIndex(diskindex.hashPrev);
                pindexNew->nHeight        = diskindex.nHeight;
                pindexNew->nFile          = diskindex.nFile;
                pindexNew->nDataPos       = diskindex.nDataPos;
                pindexNew->nUndoPos       = diskindex.nUndoPos;
                pindexNew->hashAnchor     = diskindex.hashAnchor;
                pindexNew->nVersion       = diskindex.nVersion;
                pindexNew->hashMerkleRoot = diskindex.hashMerkleRoot;
                pindexNew->hashReserved   = diskindex.hashReserved;
                pindexNew->nTime          = diskindex.nTime;
                pindexNew->nBits          = diskindex.nBits;
                pindexNew->nNonce         = diskindex.nNonce;
                pindexNew->nSolution      = diskindex.nSolution;
                pindexNew->nStatus        = diskindex.nStatus;
                pindexNew->nTx            = diskindex.nTx;
                pindexNew->nSproutValue   = diskindex.nSproutValue;

                if (!CheckProofOfWork(pindexNew->GetBlockHash(), pindexNew->nBits, Params().GetConsensus()))
                    return error("LoadBlockIndex(): CheckProofOfWork failed: %s", pindexNew->ToString());

                pcursor->Next();
            } else {
                break; // if shutdown requested or finished loading block index
            }
        } catch (const std::exception& e) {
            return error("%s: Deserialize or I/O error - %s", __func__, e.what());
        }
    }

    return true;
}
