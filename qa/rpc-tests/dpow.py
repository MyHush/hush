#!/usr/bin/env python2
# Copyright (c) 2018 The Hush developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal, initialize_chain_clean, \
    start_node, stop_node, wait_bitcoinds


class DPoWTest(BitcoinTestFramework):

    def setup_chain(self):
        print("Initializing test directory "+self.options.tmpdir)
        initialize_chain_clean(self.options.tmpdir, 1)

    def setup_network(self):
        self.nodes = []
        self.is_network_split = False
        self.nodes.append(start_node(0, self.options.tmpdir))

    def run_test(self):
        self.nodes[0].generate(3)
        rpc = self.nodes[0]

        # Verify that basic RPC functions exist and work
        result = rpc.calc_MoM(2,20)
        print result

if __name__ == '__main__':
    DPoWTest().main()
