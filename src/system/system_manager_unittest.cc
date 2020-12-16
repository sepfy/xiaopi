#include <string>
#include "gtest/gtest.h"
#include "system_manager.h"

TEST(SystemManager, Init) {
  SystemManager *system_manager = SystemManager::GetInstance();
  EXPECT_TRUE("admin" == system_manager->ReadConfig("username"));
}

TEST(SystemManager, UpdateReadConfigIfNoKey) {
  SystemManager *system_manager = SystemManager::GetInstance();
  EXPECT_TRUE("" == system_manager->ReadConfig("if_no_key"));
}

TEST(SystemManager, UpdateReadConfig) {
  std::string password("password");
  SystemManager *system_manager = SystemManager::GetInstance();
  system_manager->UpdateConfig("password", password);
  EXPECT_TRUE(password == system_manager->ReadConfig("password"));
}

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


