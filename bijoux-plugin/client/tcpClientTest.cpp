
#include "gtest/gtest.h"

int Factorial ( int a ) {
	return -5;
}

TEST(FactorialTest, Negative) {
  // This test is named "Negative", and belongs to the "FactorialTest"
  // test case.
  EXPECT_EQ(-5, Factorial(-5));
}
