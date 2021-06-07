#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "utility/utility.h"
#include "wifi_softap.h"

WifiSoftap::WifiSoftap() : WifiInterface(SOFTAP_CTRL_INTERFACE) {

  std::ifstream file("/etc/hostapd.conf");
  std::string default_config((std::istreambuf_iterator<char>(file)),
   std::istreambuf_iterator<char>());
  customized_config_ = default_config;
}

void WifiSoftap::Disable() {

  char cmd[32] = {0};
  size_t cmd_len = 0;
  char reply[8] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "DISABLE");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);

}

void WifiSoftap::Enable() {

  char cmd[32] = {0};
  size_t cmd_len = 0;
  char reply[8] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "ENABLE");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);

}

void WifiSoftap::SaveConfig() {

  std::ofstream ofs("/tmp/hostapd.conf");
  ofs << customized_config_;
  ofs.close();
}

bool WifiSoftap::UpdateConfig(std::string key, std::string value) {


  bool updated_config = false;

  std::string out_config = "";
  std::istringstream in_config(customized_config_);
  std::string line;
  while (std::getline(in_config, line)) {
    std::string substr = line.substr(0, line.find("="));
    if(substr == key) {
      std::string new_setting = key + "=" + value + "\n";
      out_config.append(new_setting);
      updated_config = true;
    }
    else {
      out_config.append(line + "\n");
    }
  }

  if(updated_config) {
    WifiSoftap::customized_config_ = out_config;
  }
  return updated_config;
}

