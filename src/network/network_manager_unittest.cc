#include <string>
#include "gtest/gtest.h"
#include "network_manager.h"

#if 0
TEST(NetworkManager, Scan) {
  NetworkManager *network_manager = NetworkManager::GetInstance();
  std::cout << network_manager->Scan();
  EXPECT_TRUE(true);
}

TEST(NetworkManager, GetInterface) {
  NetworkManager *network_manager = NetworkManager::GetInstance();
  std::string tmp("wlan0");
  EXPECT_TRUE("wlan0" == network_manager->GetInterfaces());
}
#endif

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


