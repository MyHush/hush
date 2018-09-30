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
	num_nodes = 1
        initialize_chain_clean(self.options.tmpdir, num_nodes)

    def setup_network(self):
        self.nodes = []
        self.is_network_split = False
        self.nodes.append(start_node(0, self.options.tmpdir))
        self.sync_all()

    def run_test(self):
        self.nodes[0].generate(3)
        rpc = self.nodes[0]

	result = rpc.getinfo()
	print result
	# TODO: actually do notarizations on regtest and test for specific data
	# TODO: does this have mainnet values somehow?
	assert(result['notarized'] >= 0)
	assert(result['notarizedhash'])
	assert(result['notarizedtxid'])

if __name__ == '__main__':
    DPoWTest().main()
