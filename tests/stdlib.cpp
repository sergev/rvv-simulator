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

constexpr size_t kBufSize = 1024;
using buf_type = std::vector<char>;

std::default_random_engine& GetGenerator() {
    static std::default_random_engine generator;
    return generator;
}

buf_type RandomBuf(size_t n = kBufSize) {
    buf_type buf;
    buf.reserve(n);
    std::uniform_int_distribution<int> distribution(0, 255);
    auto gen = [&]() { return static_cast<char>(distribution(GetGenerator())); };
    std::generate_n(std::back_inserter(buf), n, gen);
    return buf;
}

}  // namespace

TEST(Stdlib, test_memcpy)
{
    buf_type in_buf = RandomBuf();
    buf_type out_buf(in_buf.size());
    rvv::memcpy(&out_buf[0], &in_buf[0], in_buf.size() * sizeof(buf_type::value_type));
    EXPECT_EQ(in_buf, out_buf);
}

TEST(Stdlib, test_memcpy_backward)
{
    buf_type in_buf = RandomBuf();
    buf_type out_buf(in_buf.size());
    rvv::memcpy_backward(&out_buf[0], &in_buf[0], in_buf.size() * sizeof(buf_type::value_type));
    EXPECT_EQ(in_buf, out_buf);
}

TEST(Stdlib, test_memmove_forward)
{
    buf_type ref_buf = RandomBuf();
    buf_type tst_buf = ref_buf;
    std::memmove(&ref_buf[kBufSize / 4], &ref_buf[0], kBufSize / 2);
    rvv::memmove(&tst_buf[kBufSize / 4], &tst_buf[0], kBufSize / 2);
    EXPECT_EQ(ref_buf, tst_buf);
}

TEST(Stdlib, test_memmove_backward)
{
    buf_type ref_buf = RandomBuf();
    buf_type tst_buf = ref_buf;
    std::memmove(&ref_buf[0], &ref_buf[kBufSize / 4], kBufSize / 2);
    rvv::memmove(&tst_buf[0], &tst_buf[kBufSize / 4], kBufSize / 2);
    EXPECT_EQ(ref_buf, tst_buf);
}

class StdlibMemsetTest : public ::testing::TestWithParam<int> {};

TEST_P(StdlibMemsetTest, test_memset)
{
    int val = GetParam();
    buf_type buf(kBufSize);
    rvv::memset(&buf[0], static_cast<char>(val), buf.size() * sizeof(buf_type::value_type));
    namespace ph = std::placeholders;
    EXPECT_TRUE(std::all_of(buf.begin(), buf.end(),
        std::bind(std::equal_to<char>(), static_cast<char>(val), ph::_1)));
}

INSTANTIATE_TEST_SUITE_P(MemsetValues, StdlibMemsetTest, ::testing::Range(0, 256));
