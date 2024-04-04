#include <gtest/gtest.h>
#include "../../src/csr.h"

namespace cemu {

class CsrTest : public ::testing::Test {
protected:
    Csr csr;
};

TEST_F(CsrTest, LoadTest) {
    csr.store(MSTATUS, 0x123456789abcdef0);
    auto value = csr.load(MSTATUS);
    ASSERT_EQ(value, 0x123456789abcdef0);
}

TEST_F(CsrTest, StoreTest) {
    csr.store(MSTATUS, 0x123456789abcdef0);
    auto value = csr.load(MSTATUS);
    ASSERT_EQ(value, 0x123456789abcdef0);
}

TEST_F(CsrTest, MedelegatedTest) {
    csr.store(MEDELEG, 0x1);
    ASSERT_TRUE(csr.is_medelegated(0));
}

TEST_F(CsrTest, MidelegatedTest) {
    csr.store(MIDELEG, 0x1);
    ASSERT_TRUE(csr.is_midelegated(0));
}

}