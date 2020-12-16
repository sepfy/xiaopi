#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "utility/security.h"

#define INIT_CONIFG "{\"username\": \"admin\", \"password\": \"12345678\"}"

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

  if(access(config_path_.c_str(), F_OK ) == -1) {
    std::ofstream o(config_path_);
    o << INIT_CONIFG << std::endl;
    o.close();
  }

  std::ifstream i(config_path_);
  i >> system_config_;
  i.close();

  hmac_ = utility::Security::Hmac(system_config_["password"], key_);

}

void SystemManager::UpdateConfig(std::string key, std::string value) {

  system_config_[key] = value;
  std::ofstream o(config_path_);
  o << system_config_ << std::endl;
  o.close();
}

std::string SystemManager::ReadConfig(std::string key) {

  if(system_config_.contains(key))
    return system_config_[key];
  else
    return "";
}

void SystemManager::ResetToDefault() {
  remove(config_path_.c_str());
  Init();
}
