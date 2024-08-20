#include <gtest/gtest.h>
#include "glash/some.hpp" 

TEST(GlashTest, Addition) {
    int sum = 1 + 1;
    EXPECT_EQ(sum, 2);
}

TEST(GlashTest, FunctionTest) {
   
    int result = glash::return_two();
    EXPECT_EQ(result, 2);  
}

TEST(GlashTest, BooleanTest) {
    bool condition = true;
    EXPECT_TRUE(condition);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
