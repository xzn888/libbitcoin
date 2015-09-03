/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "bip38.hpp"

#include <algorithm>
#include <string>
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

// TODO: implement testnet tests.

#ifndef ENABLE_TESTNET

BOOST_AUTO_TEST_SUITE(bip38_tests)

///////////////////////////////////////////////////////////////////////////////

#ifdef WITH_ICU

BOOST_AUTO_TEST_SUITE(bip38__lock_secret)

static void test_lock_secret(const bip38_vector& vector)
{
    ec_secret secret;
    BOOST_REQUIRE(decode_base16(secret, vector.unlocked));

    const auto key = bip38::lock_secret(secret, vector.passphrase, vector.compressed);
    BOOST_REQUIRE_EQUAL(encode_base58(key), vector.locked);
}

BOOST_AUTO_TEST_CASE(bip38__lock__vector_0_not_multiplied_uncompressed__expected)
{
    test_lock_secret(bip38_test_vector0);
}

BOOST_AUTO_TEST_CASE(bip38__lock__vector_1_not_multiplied_uncompressed__expected)
{
    test_lock_secret(bip38_test_vector1);
}

BOOST_AUTO_TEST_CASE(bip38__lock__vector_2_not_multiplied__expected)
{
    test_lock_secret(bip38_test_vector2);
}

BOOST_AUTO_TEST_CASE(bip38__lock__vector_3_not_multiplied__expected)
{
    test_lock_secret(bip38_test_vector3);
}

// #3 from: github.com/bitcoin/bips/blob/master/bip-0038.mediawiki#no-compression-no-ec-multiply
BOOST_AUTO_TEST_CASE(bip38__lock__vector_unicode_not_multiplied_uncompressed___expected)
{
    data_chunk not_normalized;
    BOOST_REQUIRE(decode_base16(not_normalized, "cf92cc8100f0909080f09f92a9"));
    std::string passphrase(not_normalized.begin(), not_normalized.end());

    // associated address: "16ktGzmfrurhbhi6JGqsMWf7TyqK9HNAeF"
    ec_secret secret;
    BOOST_REQUIRE(decode_base16(secret, "64eeab5f9be2a01a8365a579511eb3373c87c40da6d2a25f05bda68fe077b66e"));

    const auto compressed = false;
    const auto locked = bip38::lock_secret(secret, passphrase, compressed);
    BOOST_REQUIRE_EQUAL(encode_base58(locked), "6PRW5o9FLp4gJDDVqJQKJFTpMvdsSGJxMYHtHaQBF3ooa8mwD69bapcDQn");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // WITH_ICU

///////////////////////////////////////////////////////////////////////////////

#ifdef WITH_ICU

BOOST_AUTO_TEST_SUITE(bip38__unlock_secret)

static void test_unlock_secret(const bip38_vector& vector)
{
    bip38::private_key key;
    BOOST_REQUIRE(decode_base58(key, vector.locked));

    const auto unlocked = bip38::unlock_secret(key, vector.passphrase);
    BOOST_REQUIRE_EQUAL(encode_base16(unlocked), vector.unlocked);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_0_not_multiplied_uncompressed__expected)
{
    test_unlock_secret(bip38_test_vector0);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_1_not_multiplied_uncompressed__expected)
{
    test_unlock_secret(bip38_test_vector1);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_2_not_multiplied__expected)
{
    test_unlock_secret(bip38_test_vector2);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_3_not_multiplied__expected)
{
    test_unlock_secret(bip38_test_vector3);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_4_uncompressed__expected)
{
    test_unlock_secret(bip38_test_vector4);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_5_uncompressed__expected)
{
    test_unlock_secret(bip38_test_vector5);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_6_uncompressed_confirmation__expected)
{
    test_unlock_secret(bip38_test_vector6);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_7_uncompressed_confirmation__expected)
{
    test_unlock_secret(bip38_test_vector7);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_8_uncompressed_confirmation_seeded__expected)
{
    test_unlock_secret(bip38_test_vector8);
}

BOOST_AUTO_TEST_CASE(bip38__unlock_secret__vector_9_uncompressed_confirmation_seeded__expected)
{
    test_unlock_secret(bip38_test_vector9);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // WITH_ICU

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE(bip38__create_intermediate)

static void test_create_intermediate(const bip38_vector& vector)
{
    bip38::intermediate intermediate;
    BOOST_REQUIRE(decode_base58(intermediate, vector.intermediate));

    bip38::seed seed;
    BOOST_REQUIRE(decode_base16(seed, vector.random_seed_data));

    bip38::confirmation_code confirmation_code;
    const auto locked = bip38::create_intermediate(intermediate, seed, confirmation_code, vector.compressed);
    BOOST_REQUIRE_EQUAL(encode_base58(locked), vector.locked);
    BOOST_REQUIRE_EQUAL(encode_base58(confirmation_code), vector.confirmation_code);
}

BOOST_AUTO_TEST_CASE(bip38__create_intermediate__vector_8_uncompressed__expected)
{
    // Currently failing
    test_create_intermediate(bip38_test_vector8);
}

BOOST_AUTO_TEST_CASE(bip38__create_intermediate__vector_9_uncompressed__expected)
{
    // Currently failing
    test_create_intermediate(bip38_test_vector9);
}

BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////////////////////////////////////

#ifdef WITH_ICU

BOOST_AUTO_TEST_SUITE(bip38__extract_address)

static void test_extract_address(const bip38_vector& vector)
{
    bip38::confirmation_code confirmation_code;
    BOOST_REQUIRE(decode_base58(confirmation_code, vector.confirmation_code));

    wallet::payment_address address;
    BOOST_REQUIRE(bip38::extract_address(confirmation_code, vector.passphrase, address));
    BOOST_REQUIRE_EQUAL(address.to_string(), vector.address);
}

BOOST_AUTO_TEST_CASE(bip38__extract_address__vector_8_uncompressed__expected)
{
    // Currently failing
    test_extract_address(bip38_test_vector8);
}

BOOST_AUTO_TEST_CASE(bip38__extract_address__vector_9_uncompressed__expected)
{
    // Currently failing
    test_extract_address(bip38_test_vector9);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // WITH_ICU

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()

#endif // !ENABLE_TESTNET
