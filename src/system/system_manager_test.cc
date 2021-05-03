#include <string>
#include "gtest/gtest.h"
#include "system_manager.h"

TEST(SystemManager, Init) {
  SystemManager *system_manager = SystemManager::GetInstance();
  EXPECT_TRUE("admin" == system_manager->ReadConfig("username"));
}

TEST(SystemManager, ReadConfigIfNoKey) {
  SystemManager *system_manager = SystemManager::GetInstance();
  EXPECT_TRUE("" == system_manager->ReadConfig("if_no_key"));
}

TEST(SystemManager, UpdateConfigIfNoKey) {
  SystemManager *system_manager = SystemManager::GetInstance();
  system_manager->UpdateConfig("nokey", std::string("123"));
  EXPECT_TRUE("123" == system_manager->ReadConfig("nokey"));
}

TEST(SystemManager, UpdateConfigBool) {
  SystemManager *system_manager = SystemManager::GetInstance();
  system_manager->UpdateConfig("booltest", false);
  bool val = true;
  system_manager->ReadConfig("booltest", &val);
  EXPECT_TRUE(false == val);
}

TEST(SystemManager, UpdateConfigBoolIfNoKey) {
  SystemManager *system_manager = SystemManager::GetInstance();
  bool val = true;
  EXPECT_TRUE(0 == system_manager->ReadConfig("booltest1", &val));
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


