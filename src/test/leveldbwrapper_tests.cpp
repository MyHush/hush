// Copyright (c) 2012-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "leveldbwrapper.h"
#include "uint256.h"
#include "random.h"
#include "test/test_bitcoin.h"

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost::assign; // bring 'operator+=()' into scope
using namespace boost::filesystem;

BOOST_FIXTURE_TEST_SUITE(dbwrapper_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(dbwrapper_compression)
{
    // Perform tests both with compression and without
    for (int i = 0; i < 2; i++) {
        bool compression = (bool)i;
        path ph = temp_directory_path() / unique_path();
        CLevelDBWrapper dbw(ph, (1 << 20), true, false, compression);
        char key = 'k';
        uint256 in = GetRandHash();
        uint256 res;

        BOOST_CHECK(dbw.Write(key, in));
        BOOST_CHECK(dbw.Read(key, res));
        BOOST_CHECK_EQUAL(res.ToString(), in.ToString());
    }
}

BOOST_AUTO_TEST_CASE(dbwrapper_maxopenfiles_64)
{
    path ph = temp_directory_path() / unique_path();
    CLevelDBWrapper dbw(ph, (1 << 20), true, false, false, 64);
    char key = 'k';
    uint256 in = GetRandHash();
    uint256 res;

    BOOST_CHECK(dbw.Write(key, in));
    BOOST_CHECK(dbw.Read(key, res));
    BOOST_CHECK_EQUAL(res.ToString(), in.ToString());
}

BOOST_AUTO_TEST_CASE(dbwrapper_maxopenfiles_1000)
{
    path ph = temp_directory_path() / unique_path();
    CLevelDBWrapper dbw(ph, (1 << 20), true, false, false, 1000);
    char key = 'k';
    uint256 in = GetRandHash();
    uint256 res;

    BOOST_CHECK(dbw.Write(key, in));
    BOOST_CHECK(dbw.Read(key, res));
    BOOST_CHECK_EQUAL(res.ToString(), in.ToString());
}

BOOST_AUTO_TEST_SUITE_END()