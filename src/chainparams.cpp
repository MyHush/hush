// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "base58.h"

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "ZEC";
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidySlowStartInterval = 2;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        //assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x24;
        pchMessageStart[1] = 0xe9;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0x64;
        vAlertPubKey = ParseHex("0411ef848a72e48fb2d8216c6698a030dedbef16ac6cd9d739a7b4e346735a8edf252223bac64093f49616050df642c61ae5b5e2bd3654d7dde05a5e296f79950b");
        nDefaultPort = 8888;
        //nMinerThreads = 0;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
         * >>> from pyblake2 import blake2s
         * >>> 'Zcash' + blake2s(b'Reuters:... ').hexdigest()
         */
        const char* pszTimestamp = "Zdashe540ecf100001889836c7d491a2f44e6bc6076d59e5e317255946b71be3fc516";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime    = 1479401611;
        genesis.nBits    = 0x1f07ffff;
        genesis.nNonce   = uint256S("0x1205000000000000000000000000000000000000000000000000000000000000");
        genesis.nSolution = ParseHex("0045f33727869d999e1d90de13eec508bb9eedf7c623ae0926922a749df12927967ce41f0363365e536a149ced0a25ce7258bcf4d36c9d69ea830ecafaac0a20576ee2405fa6e1ed91e5e5f3e72add8dae92175e05bb114100c654b90a0ea0e35e3ecee480d073b90c0d252963519492dce839f14bd3e54b017972fe05c30f61d73437d2ccb8a8dac1f33ba27c9d8a81bc02911a0f04f0af099ba8cc9114bd0de60fcd699673bc7a05d06baf544c5daa773991ff85c269c17f3a58618818f91d6ee9e4c393cd8d03428a2cc9be4243384caf197c77029b8c6c9387ab7a4966faac8efb13f7f9e259040fbbc1295551ad10084dba5ef0b732da7c90bd06026152acc74aad871aa3c2e46666d262129f07e128a0ea322b532851d649d2e0443954dafaaddfa6af28a70c786b94d904cb8ad3fda5364d9d5cccd1ce802f801dad25644a7dcab1c44191f24cdd8997f19c3602bc6e936aca34413a3270f837d94b211c5ad7533a070fe7b5651db5bf5d2aa13d503af5eef912fdcb5704a1bfc4c095d9b3da8f62261ed0dea089bf5b12db067ce1b4bee14f6515a180dedfeac9d794649da2b203e2a975db5c961daf788239ec722a7afbc63b8cb30c11e0ce1943f71981c3437530a4c0fcec0f9e7c6c1f5e85373ff132ddccb202916665c5559ddbcff20c743f15e2b7748a11d5de283f8cf190cb0c47feec730f6bde470ae172b9fdfc07e96362dd9238833876ff14b8028e811af7610b08842aac55f79f066e5df160104bd571f44fb16d7476e2626458c2992f6291781915d5ee927594c8a5966ba379ace56cf30a9c5f79e70fa6347e664796eea268d74862522ac275bf94ef2e406034ff121345351b92245ba834c752249d3506a111407bbd10e31b1167f9c3702fc54176e18ad8ca6211c892d4ba4598b3d834c2aa06392e21b4ff3d6d510132989a9828086bbbd1e03debe3fd44e34550c54103df97e25709e086efefc27013b9fc08bc777f96630876dfcba638d5f1f2fe10935d5fb950388ca485044535350e83267d0b5c75e5f570cd849d8567daecb90b011f72bccde31e8c6b13a630b82dc1fe68f7ef261013e2a91ed033c11fa201f4a32f7a05a58cbbce0a13f175c5e08a3ced164686a26277be7bbac9fed583276d12d67a12649b4f46e466bdc57bed60c3f8bd6e05f73d54686d36e97a6461cbda430abf9ec8df6699160822abbc947463338741c961fcaab0dbbd2a2b92084d0b372242e6103aa54479fc7b350f35c23eb1f3161352ec3e1ba178fd82147ff42ba02164405fe74606b6c525883dddc5fd8431bb49c889571c155974af19291e49f55fbfe7c823f526f77edc9bba773f9e370baf614175515eef3f7be1750ee2a3c915a0f8e3ea31bdd397668ebef7f0cf75ddecec887df2baf771e1013d2d4795c39ba8449c18d559eab9c2718f39531c0b632437506c9061ee9995e11173da85948b5b82e02e54977eb64db682af9a1993bbd16fcab3d55be7493a38ab9012d6c3f5834f3740fcc725aa70e0570b46073ad43c7b0a582af576f8e2afd4c6c75d933b0d8955a49c2b522d84a9fc9c75bbf2b5363667e5b42b420eddfa537ac5d15d7a4ea9c5dfd063cb03e81da219163fe7135d61db9d19f891e5784613a59df7ffefa2017f8d6edfde9ce19babd5963aae94ad7bfd52e736323c73f9765c9026f1b0a560dbdcc71563586f4ced072c0903bdf6362fe4f4c0cea265ad48a6639bccaa08b5593a9f0eeac5cfcc14ca6d2e7449ec029598b4061793745ba3e53dc289c16ee84c7c2575824e7a5e372d8c8e1bf0a2dfa90283a67bd305c684b39af1d109004522301344a9c346a0e227c45749aa24b11623211155bad78e1a137376738662a78619dceaf0c79f");
       
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0003a67bc26fe564b75daf11186d360652eb435a35ba3d9d3e7e5d5f8e62dc17"));
        assert(genesis.hashMerkleRoot == uint256S("0x830539f9ec196f36a2759638b674a51b668eba7bbf6af10c56fed4af666be177"));


  
        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("node1", "192.99.3.29")); // node1
        vSeeds.push_back(CDNSSeedData("node2", "zdash.suprnova.cc")); // node2
        vSeeds.push_back(CDNSSeedData("node3", "76.188.211.191")); // node3
        //vSeeds.push_back(CDNSSeedData("znodes.org", "dnsseed.znodes.org")); //

        // guarantees the first 2 characters, when base58 encoded, are "t1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xB8};
        // guarantees the first 2 characters, when base58 encoded, are "t3"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBD};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
            (120, uint256S("0x01e107af8605050c438e904a145e0e6479077bb2b4abbed8f9067adbeb37dd77")),
            1479655018, // * UNIX timestamp of last checkpoint block
            127, // * total number of transactions between genesis and last checkpoint
                 //   (the tx=... number in the SetBestChain debug.log lines)
            848 // * estimated number of transactions per day after checkpoint
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
            "t3Vz22vK5z2LcKEdg16Yv4FFneEL1zg9ojd", /* main-index: 0*/
            "t3cL9AucCajm3HXDhb5jBnJK2vapVoXsop3", /* main-index: 1*/
            "t3fqvkzrrNaMcamkQMwAyHRjfDdM2xQvDTR", /* main-index: 2*/
            "t3TgZ9ZT2CTSK44AnUPi6qeNaHa2eC7pUyF", /* main-index: 3*/
            "t3SpkcPQPfuRYHsP5vz3Pv86PgKo5m9KVmx", /* main-index: 4*/
            "t3Xt4oQMRPagwbpQqkgAViQgtST4VoSWR6S", /* main-index: 5*/
            "t3ayBkZ4w6kKXynwoHZFUSSgXRKtogTXNgb", /* main-index: 6*/
            "t3adJBQuaa21u7NxbR8YMzp3km3TbSZ4MGB", /* main-index: 7*/
            "t3K4aLYagSSBySdrfAGGeUd5H9z5Qvz88t2", /* main-index: 8*/
            "t3RYnsc5nhEvKiva3ZPhfRSk7eyh1CrA6Rk", /* main-index: 9*/
            "t3Ut4KUq2ZSMTPNE67pBU5LqYCi2q36KpXQ", /* main-index: 10*/
            "t3ZnCNAvgu6CSyHm1vWtrx3aiN98dSAGpnD", /* main-index: 11*/
            "t3fB9cB3eSYim64BS9xfwAHQUKLgQQroBDG", /* main-index: 12*/
            "t3cwZfKNNj2vXMAHBQeewm6pXhKFdhk18kD", /* main-index: 13*/
            "t3YcoujXfspWy7rbNUsGKxFEWZqNstGpeG4", /* main-index: 14*/
            "t3bLvCLigc6rbNrUTS5NwkgyVrZcZumTRa4", /* main-index: 15*/
            "t3VvHWa7r3oy67YtU4LZKGCWa2J6eGHvShi", /* main-index: 16*/
            "t3eF9X6X2dSo7MCvTjfZEzwWrVzquxRLNeY", /* main-index: 17*/
            "t3esCNwwmcyc8i9qQfyTbYhTqmYXZ9AwK3X", /* main-index: 18*/
            "t3M4jN7hYE2e27yLsuQPPjuVek81WV3VbBj", /* main-index: 19*/
            "t3gGWxdC67CYNoBbPjNvrrWLAWxPqZLxrVY", /* main-index: 20*/
            "t3LTWeoxeWPbmdkUD3NWBquk4WkazhFBmvU", /* main-index: 21*/
            "t3P5KKX97gXYFSaSjJPiruQEX84yF5z3Tjq", /* main-index: 22*/
            "t3f3T3nCWsEpzmD35VK62JgQfFig74dV8C9", /* main-index: 23*/
            "t3Rqonuzz7afkF7156ZA4vi4iimRSEn41hj", /* main-index: 24*/
            "t3fJZ5jYsyxDtvNrWBeoMbvJaQCj4JJgbgX", /* main-index: 25*/
            "t3Pnbg7XjP7FGPBUuz75H65aczphHgkpoJW", /* main-index: 26*/
            "t3WeKQDxCijL5X7rwFem1MTL9ZwVJkUFhpF", /* main-index: 27*/
            "t3Y9FNi26J7UtAUC4moaETLbMo8KS1Be6ME", /* main-index: 28*/
            "t3aNRLLsL2y8xcjPheZZwFy3Pcv7CsTwBec", /* main-index: 29*/
            "t3gQDEavk5VzAAHK8TrQu2BWDLxEiF1unBm", /* main-index: 30*/
            "t3Rbykhx1TUFrgXrmBYrAJe2STxRKFL7G9r", /* main-index: 31*/
            "t3aaW4aTdP7a8d1VTE1Bod2yhbeggHgMajR", /* main-index: 32*/
            "t3YEiAa6uEjXwFL2v5ztU1fn3yKgzMQqNyo", /* main-index: 33*/
            "t3g1yUUwt2PbmDvMDevTCPWUcbDatL2iQGP", /* main-index: 34*/
            "t3dPWnep6YqGPuY1CecgbeZrY9iUwH8Yd4z", /* main-index: 35*/
            "t3QRZXHDPh2hwU46iQs2776kRuuWfwFp4dV", /* main-index: 36*/
            "t3enhACRxi1ZD7e8ePomVGKn7wp7N9fFJ3r", /* main-index: 37*/
            "t3PkLgT71TnF112nSwBToXsD77yNbx2gJJY", /* main-index: 38*/
            "t3LQtHUDoe7ZhhvddRv4vnaoNAhCr2f4oFN", /* main-index: 39*/
            "t3fNcdBUbycvbCtsD2n9q3LuxG7jVPvFB8L", /* main-index: 40*/
            "t3dKojUU2EMjs28nHV84TvkVEUDu1M1FaEx", /* main-index: 41*/
            "t3aKH6NiWN1ofGd8c19rZiqgYpkJ3n679ME", /* main-index: 42*/
            "t3MEXDF9Wsi63KwpPuQdD6by32Mw2bNTbEa", /* main-index: 43*/
            "t3WDhPfik343yNmPTqtkZAoQZeqA83K7Y3f", /* main-index: 44*/
            "t3PSn5TbMMAEw7Eu36DYctFezRzpX1hzf3M", /* main-index: 45*/
            "t3R3Y5vnBLrEn8L6wFjPjBLnxSUQsKnmFpv", /* main-index: 46*/
            "t3Pcm737EsVkGTbhsu2NekKtJeG92mvYyoN", /* main-index: 47*/
//            "t3PZ9PPcLzgL57XRSG5ND4WNBC9UTFb8DXv", /* main-index: 48*/
//            "t3L1WgcyQ95vtpSgjHfgANHyVYvffJZ9iGb", /* main-index: 49*/
//            "t3JtoXqsv3FuS7SznYCd5pZJGU9di15mdd7", /* main-index: 50*/
//            "t3hLJHrHs3ytDgExxr1mD8DYSrk1TowGV25", /* main-index: 51*/
//            "t3fmYHU2DnVaQgPhDs6TMFVmyC3qbWEWgXN", /* main-index: 52*/
//            "t3T4WmAp6nrLkJ24iPpGeCe1fSWTPv47ASG", /* main-index: 53*/
//            "t3fP6GrDM4QVwdjFhmCxGNbe7jXXXSDQ5dv", /* main-index: 54*/
};
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "TUSH";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = true;
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x1a;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xbf;
        vAlertPubKey = ParseHex("040c2275ef276b57c646edd5d4441b911c6b166596b0ee853e67842c580b9ed14dab42c7db0985afa1454f9480004e9597178ad4c51d54069bc1013909818a7b81");
        nDefaultPort = 18888;
        //nMinerThreads = 0; #Error on 1.0.12-rc Build to 1.0.9
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1506883264;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000007");
        genesis.nSolution = ParseHex("00e76338efc84e87b18b92ae132b31a6ff581ff89f1aeacf5c650f46ba9b6db78640fbb7f1ebf417ac890f747e4965f231ad966795d88eb3fab219fcb91d664b24e26d62e6572bc28289d058fcad472cc91ce83102560a46f8c2bef9f3865407beff0d3de381d0c0330f0e10fa1b052c495a9a2669edf9a385ab18359ed61953dc000c870899653ee2b391bd5930cc134e1bea41cb64bae2262cb186b12613df6098c9a062ff23690367c5698159c7aae04e03fcad58686e0cda1223da0cd4aeb761628f77999a52e1b1d58dee4c5f1d283c18d4264e5ebb73b5e8a241905ab2a38097157508902b9ede96e32ccf27bd5fa41cdaf65eed6ff54e26340726d8abdb207363e176c29798a9b5e94b0eab5a56076b95a24659daa7cbfd70897f06c891a5b673e98c29638ed5fcdecacfa4cf3b30a47413c71b1a5b57c240ae3cd001a9753b8e0af92cddeea7f28edffb3ae304e307e83ad7d4ed899972f1a4d8b98d8ec0bee6b40e594c15936b9d89c2e1c485087cae539d97bea3a14537daa89dabd5dfb45ea7266def7baafcbf5a6a3150f506d810333a2da6d8eb4f2e6de036dce45d95880612bfcbe9e446a954c133746f7cc5deebb27b21e8199603ec0eb0ee41d2c0a22f1363622ccfe672bfd00d566cdfdc4c4555ea3baa9662eae5ef399ebb345042ad4a607fd78c615484b66746eb2e56edf07b79c9077df6370fa17d91f8c4036fc3afb7b2065598a169142f12c75725986b3d7b33768fcb87de68ecfa924308cea135fb0feb5121d82538dd5378425beedcc36c21d8266e6c1c6147dd22c27f90a80f2cf8b5160f830785250422b51857cdf6b1d5851a08126dbebc957a0816a4f914a4a02b48e813f64745fcf1093a6cf9660f3d7e556c87fb55d98d420fe02497d11dbf5191310f7abe28d8c7ebb28f7ae702d0d34de236e11ecbcd028008c78821db534c8f36e6ff633d8eb73057967a0bf41b9b0dc3af5856e6325522513279924fba9d61035bce23c89d39151d4a907e0b9b407dc5c3daae4c0f3f4c996d4de800b696a285edc2d13d33616bc33d14fb15900f97b6f9b8c83326471f12c540829581f14e814ec121a922f9bf10e9f8ba7359d6be99fc8a3d2b65d3a691f22e45af9a84ce0332930944fe4ad535634e9c65af25c0a1ca965a3f9b77cf9f37195e5c8c04d19999d828706b6fad344f775ccdc17a60de507f3d4c866ea2d38e37ceae749038c56962e258f84683149948aded24e461c5b682c68efcfa263e631eca06453c926e39ed0443aa4de6b70dbb13061114b44d2f10f7e0b4be5f31559764f1f92175029164e19f595712bdbc51aad5d445537aa392d543e719fb167267d62d933e96286698f39ef7c370adfafd423f95fff9112eabde6eca5efb5b107fa3e8f9c7147628ccfedadb03b9e43712a88505dfe620f1eb6610fcdf4fcefcd20ec4f429d8f2dd17e8a6248b982ebc2ddf663ad7f90af22fb325534505995e555c53ae80e26240ff844a0f71050ef6b6ddb5b88c551f9759950622bc9a012006224f0d4e5cbdf118293190c74f4f9e5617d445e145a24cb19061c0bff7223b77bee96b7fbe89feef5b255ffda27df3a975cf57757974f591b5abadb0138e3d2fcc522beb9c2da51d94470e3f25a58b75d7ad01041bec72c021f6737741f1360ba0c2d269d35b0fb81b75a7cb855372a1f8e0494614db52cf04719ad78f0c427eb23ee3c8efe797364b94ea11c643db1aa3c01a2b16c46a888c6472b41648b74b829211e9f55d3c175e57906155a6174ac164345fed374dc4b1721732258f742dbd4ab0bee777c4f2b6781cda1ed0d90da21c9c11e85749833ad143c3c65231bb3e39325a7799342dea78e055383789d6136d12620caea3ecda8ed3");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x" + consensus.hashGenesisBlock.ToString()));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("tush1", "tush.cryptominingpools.org")); // Zcash
        vSeeds.push_back(CDNSSeedData("node1", "testnet.myhush.network")); // madbuda
        vSeeds.push_back(CDNSSeedData("node2", "testnet.madmining.club")); // madmining

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock),
            1506883264,  // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                         //   (the tx=... number in the SetBestChain debug.log lines)
            1          //   total number of tx / (checkpoint block height / (24 * 24))
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
            "t2UNzUUx8mWBCRYPRezvA363EYXyEpHokyi", "t2N9PH9Wk9xjqYg9iin1Ua3aekJqfAtE543", "t2NGQjYMQhFndDHguvUw4wZdNdsssA6K7x2", "t2ENg7hHVqqs9JwU5cgjvSbxnT2a9USNfhy",
            "t2BkYdVCHzvTJJUTx4yZB8qeegD8QsPx8bo", "t2J8q1xH1EuigJ52MfExyyjYtN3VgvshKDf", "t2Crq9mydTm37kZokC68HzT6yez3t2FBnFj", "t2EaMPUiQ1kthqcP5UEkF42CAFKJqXCkXC9", 
            "t2F9dtQc63JDDyrhnfpzvVYTJcr57MkqA12", "t2LPirmnfYSZc481GgZBa6xUGcoovfytBnC", "t26xfxoSw2UV9Pe5o3C8V4YybQD4SESfxtp", "t2D3k4fNdErd66YxtvXEdft9xuLoKD7CcVo", 
            "t2DWYBkxKNivdmsMiivNJzutaQGqmoRjRnL", "t2C3kFF9iQRxfc4B9zgbWo4dQLLqzqjpuGQ", "t2MnT5tzu9HSKcppRyUNwoTp8MUueuSGNaB", "t2AREsWdoW1F8EQYsScsjkgqobmgrkKeUkK", 
            "t2Vf4wKcJ3ZFtLj4jezUUKkwYR92BLHn5UT", "t2K3fdViH6R5tRuXLphKyoYXyZhyWGghDNY", "t2VEn3KiKyHSGyzd3nDw6ESWtaCQHwuv9WC", "t2F8XouqdNMq6zzEvxQXHV1TjwZRHwRg8gC", 
            "t2BS7Mrbaef3fA4xrmkvDisFVXVrRBnZ6Qj", "t2FuSwoLCdBVPwdZuYoHrEzxAb9qy4qjbnL", "t2SX3U8NtrT6gz5Db1AtQCSGjrpptr8JC6h", "t2V51gZNSoJ5kRL74bf9YTtbZuv8Fcqx2FH", 
            "t2FyTsLjjdm4jeVwir4xzj7FAkUidbr1b4R", "t2EYbGLekmpqHyn8UBF6kqpahrYm7D6N1Le", "t2NQTrStZHtJECNFT3dUBLYA9AErxPCmkka", "t2GSWZZJzoesYxfPTWXkFn5UaxjiYxGBU2a", 
            "t2RpffkzyLRevGM3w9aWdqMX6bd8uuAK3vn", "t2JzjoQqnuXtTGSN7k7yk5keURBGvYofh1d", "t2AEefc72ieTnsXKmgK2bZNckiwvZe3oPNL", "t2NNs3ZGZFsNj2wvmVd8BSwSfvETgiLrD8J", 
            "t2ECCQPVcxUCSSQopdNquguEPE14HsVfcUn", "t2JabDUkG8TaqVKYfqDJ3rqkVdHKp6hwXvG", "t2FGzW5Zdc8Cy98ZKmRygsVGi6oKcmYir9n", "t2DUD8a21FtEFn42oVLp5NGbogY13uyjy9t", 
            "t2UjVSd3zheHPgAkuX8WQW2CiC9xHQ8EvWp", "t2TBUAhELyHUn8i6SXYsXz5Lmy7kDzA1uT5", "t2Tz3uCyhP6eizUWDc3bGH7XUC9GQsEyQNc", "t2NysJSZtLwMLWEJ6MH3BsxRh6h27mNcsSy", 
            "t2KXJVVyyrjVxxSeazbY9ksGyft4qsXUNm9", "t2J9YYtH31cveiLZzjaE4AcuwVho6qjTNzp", "t2QgvW4sP9zaGpPMH1GRzy7cpydmuRfB4AZ", "t2NDTJP9MosKpyFPHJmfjc5pGCvAU58XGa4", 
            "t29pHDBWq7qN4EjwSEHg8wEqYe9pkmVrtRP", "t2Ez9KM8VJLuArcxuEkNRAkhNvidKkzXcjJ", "t2D5y7J5fpXajLbGrMBQkFg2mFN8fo3n8cX", "t2UV2wr1PTaUiybpkV3FdSdGxUJeZdZztyt", 
            };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0x3f;
        pchMessageStart[3] = 0x5f;
        //nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000009");
        genesis.nSolution = ParseHex("01936b7db1eb4ac39f151b8704642d0a8bda13ec547d54cd5e43ba142fc6d8877cab07b3");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        assert(consensus.hashGenesisBlock == uint256S("0x" + consensus.hashGenesisBlock.ToString()));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")),
            0,
            0,
            0
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = { "t2FwcEhFdNXuFMv1tcYwaBJtYVtMj8b1uTg" };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CBitcoinAddress address(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(address.IsValid());
    assert(address.IsScript());
    CScriptID scriptID = get<CScriptID>(address.Get()); // Get() returns a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}
