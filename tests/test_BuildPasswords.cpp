#include <gtest/gtest.h>

#include "BuildPasswords.h"

class BuildPasswordsTest : public ::testing::Test {
protected:
    BuildPasswords buildpasswords;
    std::array<int, 3> zeros;
    std::array<int, 3> zeros_reversed;
    std::vector<unsigned char> buffer_15;
    std::vector<unsigned char> buffer_10;

    void SetUp() override {
        buffer_15 = {'1','2','3','4','5','6','7','8','9','0','a','b','c','d','e'};
        buffer_10 = {'1','2','3','4','5','6','7','8','9','0'};
        zeros = {1,2,3};
        zeros_reversed = {3,2,1};
    }
};

TEST_F(BuildPasswordsTest, ReturnsZeroAtZeros) {
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(1.0, zeros), 0.0);
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(2.0, zeros), 0.0);
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(3.0, zeros), 0.0);
}

TEST_F(BuildPasswordsTest, ReturnsValueOutsideZeros) {
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(0.0, zeros), -6.0);
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(4.0, zeros), 6.0);
}

TEST_F(BuildPasswordsTest, ReturnsValuesBetweenZeros) {
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(1.5, zeros), 0.375);
    EXPECT_DOUBLE_EQ(buildpasswords.evaluateCubicFx(2.5, zeros), -0.375);
}

TEST_F(BuildPasswordsTest, ReturnsCriticalPoints) {
    std::array<double, 2> result = buildpasswords.findCriticalPoints(zeros);

    EXPECT_NEAR(result[0], 1.42265, 1E-4);
    EXPECT_NEAR(result[1], 2.57735, 1E-4);
}

TEST_F(BuildPasswordsTest, ReturnsCriticalPointsBetweenZeros) {
    std::array<double, 2> result = buildpasswords.findCriticalPoints(zeros);

    EXPECT_GT(result[0], zeros[0]);
    EXPECT_LT(result[0], zeros[1]);
    EXPECT_GT(result[1], zeros[1]);
    EXPECT_LT(result[1], zeros[2]);
}

TEST_F(BuildPasswordsTest, ReturnsSameCriticalPointsDifferentOrderedZeros) {
    std::array<double, 2> result1 = buildpasswords.findCriticalPoints(zeros);
    std::array<double, 2> result2 = buildpasswords.findCriticalPoints(zeros_reversed);

    EXPECT_NEAR(result1[0], result2[0], 1E-10);
    EXPECT_NEAR(result1[1], result2[1], 1E-10);
}

TEST_F(BuildPasswordsTest, ReturnsThreeUniqueZeros) {
    std::array<int, 3> unique_zeros = buildpasswords.generateUniqueZeros(buffer_15);

    EXPECT_EQ(unique_zeros.size(), 3);

    for (int z : unique_zeros) {
        EXPECT_GE(z, 1);
        EXPECT_LE(z, 1000);
    }

    EXPECT_NE(zeros[0], zeros[1]);
    EXPECT_NE(zeros[0], zeros[2]);
    EXPECT_NE(zeros[1], zeros[2]);
}

TEST_F(BuildPasswordsTest, ReturnsSameZerosGivenSameBuffer) {
    std::array<int, 3> result1 = buildpasswords.generateUniqueZeros(buffer_15);
    std::array<int, 3> result2 = buildpasswords.generateUniqueZeros(buffer_15);

    EXPECT_EQ(result1, result2);
}

TEST_F(BuildPasswordsTest, ReturnsDifferentZerosGivenDifferentBuffer) {
    std::array<int, 3> result1 = buildpasswords.generateUniqueZeros(buffer_15);
    std::array<int, 3> result2 = buildpasswords.generateUniqueZeros(buffer_10);

    EXPECT_NE(result1, result2);
}

TEST_F(BuildPasswordsTest, ReturnsThreeSaltsInProperRange) {
    std::array<int, 3> result = buildpasswords.getSaltIndices(buffer_15);

    EXPECT_EQ(result.size(), 3);

    for (int s : result) {
        EXPECT_GE(s, 1);
        EXPECT_LE(s, 10);
    }

    EXPECT_NE(result[0], result[1]);
    EXPECT_NE(result[0], result[2]);
    EXPECT_NE(result[1], result[2]);
}