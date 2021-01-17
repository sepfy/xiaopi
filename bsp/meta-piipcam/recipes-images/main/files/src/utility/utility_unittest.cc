#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "utility/net.h"


TEST(UtilityNet, GetIpAddr) {

  std::cout << utility::Net::GetIpAddr("wlx40b07656eff8") << std::endl;
  EXPECT_TRUE(1);
}

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


