#include <gtest/gtest.h>
#include <QDir>
#include <QSettings>

#include "utils.h"

class UtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::tempPath());
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings testSettings("BCrypticTest", "BCrypticAppTest");
        testSettings.clear();
        testSettings.sync();
    }

    void TearDown() override {
        QSettings testSettings("BCrypticTest", "BCrypticAppTest");
        testSettings.clear();
    }
};

TEST_F(UtilsTest, ReturnsFalseByDefault) {
    EXPECT_FALSE(Utils::isAppConfigured("BCrypticTest", "BCrypticAppTest"));
}

TEST_F(UtilsTest, ReturnTrueAfterMarkingAsConfigured) {
    Utils::markAsConfigured("BCrypticTest", "BCrypticAppTest");
    
    EXPECT_TRUE(Utils::isAppConfigured("BCrypticTest", "BCrypticAppTest"));
}

TEST_F(UtilsTest, ReturnFalseAfterMarkingUnconfigured) {
    Utils::markAsConfigured("BCrypticTest", "BCrypticAppTest");
    Utils::markAsUnconfigured("BCrypticTest", "BCrypticAppTest");
    
    EXPECT_FALSE(Utils::isAppConfigured("BCrypticTest", "BCrypticAppTest"));
}

TEST_F(UtilsTest, ReturnsRandomBytesSizeofInput) {
    size_t length = 12;
    std::vector<unsigned char> randomBytes = Utils::generateRandomBytes(length);
    
    EXPECT_EQ(randomBytes.size(), length);
}