#include <gtest/gtest.h>
#include "utils.h"
#include "vault.h"

class VaultTest : public ::testing::Test {
protected:
    unsigned long long opslimit;
    size_t memlimit;
    Vault vault;

    void SetUp() override {
        opslimit = 3;
        memlimit = 268435456;
    }
};

TEST_F(VaultTest, ReturnsAssociatedData) {
    SUCCEED();
}