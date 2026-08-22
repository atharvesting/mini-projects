#include <gtest/gtest.h>

struct SpenvTests
    : public ::testing::Test
{
    virtual void SetUp() override {

    }

    virtual void TearDown() override {
        
    }
};

TEST_F(SpenvTests, TestTest) {
    EXPECT_TRUE(true);
}