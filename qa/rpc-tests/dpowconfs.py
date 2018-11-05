#!/usr/bin/env python2
# Copyright (c) 2018 The Hush developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal, initialize_chain_clean, \
    start_node, stop_node, wait_bitcoinds


class DPoWConfsTest(BitcoinTestFramework):

    def setup_chain(self):
        print("Initializing test directory "+self.options.tmpdir)
        num_nodes = 1
        initialize_chain_clean(self.options.tmpdir, num_nodes)

    def setup_network(self):
        self.nodes = []
        self.is_network_split = False
        self.nodes.append(start_node(0, self.options.tmpdir))
        self.sync_all()

    def run_test(self):
        rpc = self.nodes[0]
        # 98 is notarized, next will be 105. Must mine at least 101
        # blocks for 100 block maturity rule
        rpc.generate(101)
        print rpc.getinfo()

        taddr = rpc.getnewaddress()
        rpc.sendtoaddress(taddr, 1987.420)
        rpc.generate(2)
        print rpc.getinfo()
        result = rpc.listunspent()

        # this xtn has 2 raw confs, but not in a notarized block,
        # so dpowconfs holds it at 1
        for res in result:
            if (res['address'] == taddr and res['generated'] == 'false'):
                assert_equal( result[0]['confirmations'], 1 )
                assert_equal( result[0]['rawconfirmations'], 2 )


if __name__ == '__main__':
    DPoWConfsTest().main()
