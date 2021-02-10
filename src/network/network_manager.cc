#include <string>
#include <fstream>
#include <vector>

#include "network/network_manager.h"
#include "utility/utility.h"
#include "system/system_manager.h"

static NetworkManager *instance_ = nullptr;

NetworkManager::NetworkManager() {
  wifi_station_.Subscribe(&wifi_softap_);
}

NetworkManager* NetworkManager::GetInstance() {
  if(instance_ == nullptr)
    instance_ = new NetworkManager();

  return instance_;
}

void NetworkManager::WifiMonitor() {

  wifi_station_.StartMonitor();
  wifi_softap_.StartMonitor();
};

std::vector<WpaInfo> NetworkManager::Scan(void) {

  std::string scan_results;
  scan_results = wifi_station_.GetScanResults();
  PLOGI("%s", scan_results.c_str());

  auto wpa_infos_str = utility::Parser::Split(scan_results, "\n");
  std::vector<WpaInfo> wpa_infos;

  for(int i = 1; i < wpa_infos_str.size() - 1; ++i) {
    auto wpa_info_str = utility::Parser::Split(wpa_infos_str[i], "\t");
    if(wpa_info_str.size() < 5)
      break;
    WpaInfo wpa_info;
    wpa_info.freq = std::stoi(wpa_info_str[1]);
    wpa_info.signal = std::stoi(wpa_info_str[2]);
    wpa_info.bssid = wpa_info_str[0];
    wpa_info.flags = wpa_info_str[3];
    wpa_info.ssid = wpa_info_str[4];
    wpa_infos.push_back(wpa_info);    
  }
  return wpa_infos;
}

WpaStatus NetworkManager::GetWpaStatus() {

  WpaStatus wpa_status;
  wpa_status.freq = 0;

  std::string wpa_status_str = wifi_station_.GetStatus();

  auto wpa_status_details = utility::Parser::Split(wpa_status_str, "\n");

  for(int i = 0; i < wpa_status_details.size(); ++i) {
    auto wpa_status_detail = utility::Parser::Split(wpa_status_details[i], "=");
    if(wpa_status_detail.size() == 2) {
      if(wpa_status_detail[0] == "freq")
        wpa_status.freq = std::stoi(wpa_status_detail[1]);
      else if(wpa_status_detail[0] == "ssid") 
        wpa_status.ssid = wpa_status_detail[1];
      else if(wpa_status_detail[0] == "bssid") 
        wpa_status.bssid = wpa_status_detail[1];
      else if(wpa_status_detail[0] == "key_mgmt") 
        wpa_status.key_mgmt = wpa_status_detail[1];
      else if(wpa_status_detail[0] == "wpa_state")
        wpa_status.wpa_state = wpa_status_detail[1];

    }
  }
  return wpa_status;
}

void NetworkManager::AddNetwork(std::string ssid, std::string psk, std::string security) {
  wifi_station_.AddNetwork(ssid, psk, security);
}

std::string NetworkManager::GetStationStatus(void) {
  return utility::Net::GetIpAddr("wlan0");
}

std::string NetworkManager::GetSoftapStatus(void) {
  return utility::Net::GetIpAddr("wlan1");
}

std::string NetworkManager::GetUsbEthernetStatus(void) {
  return utility::Net::GetIpAddr("usb0");
}

std::string NetworkManager::GetStationNetmask(void) {
  return utility::Net::GetNetmask("wlan0");
}

std::string NetworkManager::GetStationHwAddr(void) {
  return utility::Net::GetHwAddr("wlan0");
}

