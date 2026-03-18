/**
    @file stdlib.cpp
    @copyright ©2019 Syntacore.
    @authors
        Grigory Okhotnikov <go@syntacore.com>
    @brief Vector extension simulator (v0.7) standard library tests
*/

#include <gtest/gtest.h>

#include "vstdlib/vstdlib.hpp"

#include <algorithm>
#include <functional>
#include <random>
#include <vector>

namespace {
    std::default_random_engine generator;
}  // namespace

TEST(Stdlib, test_memcpy)
{
    using std::begin;
    using std::end;
    typedef std::vector<char> buf_type;
    buf_type in_buf;
    static std::uniform_int_distribution<int> distribution(0, 255);
    static auto const gen = []() {return distribution(generator); };
    std::generate_n(std::back_inserter(in_buf), 1024, gen);
    buf_type out_buf(in_buf.size());
    rvv::memcpy(&out_buf[0], &in_buf[0], in_buf.size() * sizeof(buf_type::value_type));
    EXPECT_EQ(in_buf, out_buf);
}

TEST(Stdlib, test_memcpy_backward)
{
    typedef std::vector<char> buf_type;

    static std::uniform_int_distribution<int> distribution(0, 255);
    static auto const gen = []() {return distribution(generator); };

    buf_type in_buf;
    using std::begin;
    using std::end;
    std::generate_n(std::back_inserter(in_buf), 1024, gen);

    buf_type out_buf(in_buf.size());
    rvv::memcpy_backward(&out_buf[0], &in_buf[0], in_buf.size() * sizeof(buf_type::value_type));
    EXPECT_EQ(in_buf, out_buf);
}

TEST(Stdlib, test_memmove_forward)
{
    typedef std::vector<char> buf_type;
    static std::uniform_int_distribution<int> distribution(0, 255);
    static auto const gen = []() {return distribution(generator); };

    buf_type ref_buf;
    using std::begin;
    using std::end;
    static size_t const buf_size = 1024;
    std::generate_n(std::back_inserter(ref_buf), buf_size, gen);

    buf_type tst_buf = ref_buf;

    std::memmove(&ref_buf[buf_size / 4], &ref_buf[0], buf_size / 2);
    rvv::memmove(&tst_buf[buf_size / 4], &tst_buf[0], buf_size / 2);
    EXPECT_EQ(ref_buf, tst_buf);
}

TEST(Stdlib, test_memmove_backward)
{
    static size_t const buf_size = 1024;
    typedef std::vector<char> buf_type;
    static std::uniform_int_distribution<int> distribution(0, 255);
    static auto const gen = []() {return distribution(generator); };

    buf_type ref_buf;
    using std::begin;
    using std::end;
    std::generate_n(std::back_inserter(ref_buf), buf_size, gen);

    buf_type tst_buf = ref_buf;

    std::memmove(&ref_buf[0], &ref_buf[buf_size / 4], buf_size / 2);
    rvv::memmove(&tst_buf[0], &tst_buf[buf_size / 4], buf_size / 2);
    EXPECT_EQ(ref_buf, tst_buf);
}

class StdlibMemsetTest : public ::testing::TestWithParam<int> {};

TEST_P(StdlibMemsetTest, test_memset)
{
    int val = GetParam();
    typedef std::vector<char> buf_type;
    buf_type buf(1024);
    rvv::memset(&buf[0], static_cast<char>(val), buf.size() * sizeof(buf_type::value_type));

    using std::begin;
    using std::end;
    namespace ph = std::placeholders;
    EXPECT_TRUE(std::all_of(begin(buf), end(buf), std::bind(std::equal_to<char>(), static_cast<char>(val), ph::_1)));
}

INSTANTIATE_TEST_SUITE_P(MemsetValues, StdlibMemsetTest, ::testing::Range(0, 256));
