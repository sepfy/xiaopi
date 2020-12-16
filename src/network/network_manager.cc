#include <string>
#include <fstream>

#include "network/network_manager.h"
#include "utility/utility.h"
#include "system/system_manager.h"

static NetworkManager *instance_ = nullptr;

NetworkManager::NetworkManager() {
}

NetworkManager* NetworkManager::GetInstance() {
  if(instance_ == nullptr)
    instance_ = new NetworkManager();

  return instance_;
}

std::string NetworkManager::Scan(void) {

  json response = json::array();
  std::size_t start, end;
  std::string cmd = "iw " + interface_ + " scan";
  std::string result = utility::Caller::PipeCall(cmd);
  auto hotspots = utility::Parser::Split(result, "(on ");

  std::string ssid;
  std::string security;
  std::string strength;
  std::string status;
  json j;
  int sig;

  for(int i = 1; i < hotspots.size(); i++) {
    start = hotspots[i].find("SSID:") + 6;
    if(start != std::string::npos) {
      end = hotspots[i].substr(start).find("\n");
      ssid = hotspots[i].substr(start, end);
    }

    start = hotspots[i].find("signal:") + 8;
    if(start != std::string::npos) {
      end = hotspots[i].substr(start).find("\n");
      sig = std::stof(hotspots[i].substr(start, end));
      if(sig > -50)
        strength = "4";
      else if(sig > -70)
        strength = "3";
      else if(sig > -80)
        strength = "2";
      else
        strength = "1";

    }

    start = hotspots[i].find("PSK");
    if(start != std::string::npos) {
      security = "WPA-PSK";
    }
    else {
      security = "None";
    }

    if(hotspots[i].find("associated") != std::string::npos) {
      status = "associated";
    }
    else {
      status = "None";
    }

    j = {{"ssid", ssid}, {"strength", strength}, {"security", security}, {"status", status}};
    response.push_back(j);
  }

  return response.dump();
}

bool NetworkManager::ApplyWpaConfig(std::string ssid, std::string psk, std::string security) {

  std::string config;
  char buf[512] = {0};
  sprintf(buf, "network={\n  ssid=\"%s\"\n  psk=\"%s\"\n  key_mgmt=%s\n}\n",
   ssid.c_str(), psk.c_str(), security.c_str());

  std::ofstream wpa_config;
  wpa_config.open("/etc/wpa_supplicant.conf", std::ios::out);

  if(wpa_config.is_open()) {
    wpa_config << buf;
    wpa_config.close();
    return true;
  }
  else 
    return false;
}

std::string NetworkManager::GetNetworkStatus(void) {
  return utility::Net::GetIpAddr(interface_);
}

