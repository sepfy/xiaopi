#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "utility/security.h"

json kDefaultConfig = {
 {"username", "admin"},
 {"password", "password"},
 {"remoteEnable", false},
 {"deviceCode", ""},
 {"deviceKey", "000000"},
};

SystemManager* SystemManager::instance_ = nullptr;

SystemManager* SystemManager::GetInstance() {
  if(instance_ == nullptr) {
    instance_ = new SystemManager();
    instance_->Init();
  }
  return instance_;
}

SystemManager::SystemManager() {

}

void SystemManager::Init() {

#ifdef DEVEL
  remove(config_path_.c_str());
#endif

  if(access(config_path_.c_str(), F_OK ) == -1) {
    std::ofstream o(config_path_);
    o << kDefaultConfig << std::endl;
    o.close();
  }

  std::ifstream i(config_path_);
  i >> system_config_;
  i.close();

  hmac_ = utility::Security::Hmac(system_config_["password"], salt_);

}


void SystemManager::UpdateConfig(std::string key, bool value) {

  system_config_[key] = value;
  std::ofstream o(config_path_);
  o << system_config_ << std::endl;
  o.close();
}

void SystemManager::UpdateConfig(std::string key, std::string value) {

  system_config_[key] = value;
  std::ofstream o(config_path_);
  o << system_config_ << std::endl;
  o.close();
}

std::string SystemManager::ReadConfig(std::string key) {
  if(system_config_[key] != nullptr)
    return system_config_[key];
  else
    return "";
}


bool SystemManager::ReadConfig(std::string key, bool *value) {
  // TODO: Check key is empty
  if(system_config_[key] != nullptr) {
    *value = system_config_[key];
    return 1;
  }
  else
    return 0;
}

void SystemManager::ResetToDefault() {
  remove(config_path_.c_str());
  Init();
}
