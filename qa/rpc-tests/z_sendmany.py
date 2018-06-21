#!/usr/bin/env python2
# Copyright (c) 2014 The Bitcoin Core developers
# Copyright (c) 2018 The Hush developers

from test_framework.test_framework import BitcoinTestFramework
from test_framework.authproxy import JSONRPCException
from test_framework.util import assert_equal, assert_greater_than, \
    initialize_chain_clean, start_nodes, start_node, connect_nodes_bi, \
    stop_nodes, sync_blocks, sync_mempools, wait_bitcoinds

import logging
import time
from decimal import Decimal

logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.INFO)

class WalletTest (BitcoinTestFramework):

    def setup_chain(self):
        print("Initializing test directory "+self.options.tmpdir)
        initialize_chain_clean(self.options.tmpdir, 4)

    def setup_network(self, split=False):
        self.nodes = start_nodes(2, self.options.tmpdir)
        connect_nodes_bi(self.nodes,0,1)
        connect_nodes_bi(self.nodes,1,2)
        self.is_network_split=False
        self.sync_all()

    def run_test (self):
        print "Mining blocks..."

        self.nodes[0].generate(4)
        self.sync_all()
        self.nodes[1].generate(20)
        self.sync_all()

        myzaddr     = self.nodes[0].z_getnewaddress()
        recipients  = [ {"address": myzaddr, "amount": Decimal('0.0') } ]
        errorString = ''

        # Make sure that amount=0 transactions can use the default fee
        # without triggering "absurd fee" errors
        try:
            myopid = self.nodes[0].z_sendmany(myzaddr, recipients)
            assert(myopid)
        except JSONRPCException,e:
            errorString = e.error['message']
            assert(False, errorString)

        # This fee is larger than the default fee and since amount=0
        # it should trigger error
        fee         = Decimal('0.1')
        recipients  = [ {"address": myzaddr, "amount": Decimal('0.0') } ]
        minconf     = 1
        errorString = ''

        try:
            myopid = self.nodes[0].z_sendmany(myzaddr, recipients, minconf, fee)
        except JSONRPCException,e:
            errorString = e.error['message']
            assert('Small transaction amount' in errorString)

        #### This fee is less than default and greater than amount, but still valid
        fee         = Decimal('0.0000001')
        recipients  = [ {"address": myzaddr, "amount": Decimal('0.00000001') } ]
        minconf     = 1
        errorString = ''

        try:
            myopid = self.nodes[0].z_sendmany(myzaddr, recipients, minconf, fee)
            assert(False, myopid)
        except JSONRPCException,e:
            errorString = e.error['message']
            assert(False, errorString)

        ### Make sure amount=0, fee=0 transaction are valid to add to mempool
        # though miners decide whether to add to a block
        fee         = Decimal('0.0')
        minconf     = 1
        recipients  = [ {"address": myzaddr, "amount": Decimal('0.0') } ]
        errorString = ''

        try:
            myopid = self.nodes[0].z_sendmany(myzaddr, recipients, minconf, fee)
            assert(myopid)
        except JSONRPCException,e:
            errorString = e.error['message']
            assert(False, errorString)


if __name__ == '__main__':
    WalletTest ().main ()
