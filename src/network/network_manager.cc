#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>

#include "network/network_manager.h"
#include "utility/utility.h"
#include "system/system_manager.h"

std::map <int, int> kWifiChannelFreqMap
{
  std::make_pair (2412, 1),
  std::make_pair (2417, 2),
  std::make_pair (2422, 3),
  std::make_pair (2427, 4),
  std::make_pair (2432, 5),
  std::make_pair (2437, 6),
  std::make_pair (2442, 7),
  std::make_pair (2447, 8),
  std::make_pair (2452, 9),
  std::make_pair (2457, 10),
  std::make_pair (2462, 11),
  std::make_pair (2467, 12),
  std::make_pair (2462, 13),
};

static NetworkManager *instance_ = nullptr;

NetworkManager::NetworkManager() {

  if(access(network_config_file_.c_str(), F_OK ) == -1) {
    network_config_ = {{"channel", 6}};
  }
  else { 
    std::ifstream i(network_config_file_);
    i >> network_config_;
    i.close();
  }

  restart_time_ms_ = -60*1000;

  std::string hwaddr = utility::Net::GetHwAddr("wlan0");
  std::string device_name = "xiaopi_" +  hwaddr.substr(0,2) + hwaddr.substr(3,2) + hwaddr.substr(6,2);

  wifi_softap_ = std::make_shared<WifiSoftap>();
  wifi_softap_->UpdateConfig("ssid", device_name);

  int channel = network_config_["channel"];
  if(channel != 6 ) {
    wifi_softap_->UpdateConfig("channel", std::to_string(channel));
  }
  wifi_softap_->SaveConfig();
  system("hostapd /tmp/hostapd.conf -B");
  wifi_softap_->Init();

  wifi_station_ = std::make_shared<WifiStation>();
  wifi_station_->Subscribe(this);
  wifi_station_->Init();
  wifi_station_->Enable();

}

NetworkManager* NetworkManager::GetInstance() {
  if(instance_ == nullptr)
    instance_ = new NetworkManager();

  return instance_;
}

void NetworkManager::WifiMonitor() {

  wifi_station_->StartMonitor();
  wifi_softap_->StartMonitor();
};

std::vector<WpaInfo> NetworkManager::Scan(void) {

  std::string scan_results;
  scan_results = wifi_station_->GetScanResults();
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

  std::string wpa_status_str = wifi_station_->GetStatus();
//  PLOGI("%s", wpa_status_str.c_str());
  auto wpa_status_details = utility::Parser::Split(wpa_status_str, "\n");

  for(int i = 0; i < wpa_status_details.size(); ++i) {
    auto wpa_status_detail = utility::Parser::Split(wpa_status_details[i], "=");
//    PLOGI("%s", wpa_status_details[i].c_str());
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
  wifi_station_->AddNetwork(ssid, psk, security);
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

void NetworkManager::OnEvent(char *buf, int len) {


  char cmd[64] = {0};
  char reply[256] = {0};
  size_t reply_len = sizeof(reply);


  if(strstr(buf, "Trying to associate") != nullptr) {

    PLOGI("Station is connected");
    if((utility::time::getms() - restart_time_ms_) < 60*1000) {
      return;
    }

    WpaStatus wpa_status = GetWpaStatus();
    int station_channel = kWifiChannelFreqMap[wpa_status.freq];
    if(station_channel == 0 || station_channel == network_config_["channel"]) {
      PLOGI("Channel does not need to change");
      return;
    }

    network_config_["channel"] = station_channel;
    PLOGI("Change channel of softap to %d", station_channel);
    wifi_softap_->UpdateConfig("channel", std::to_string(station_channel));
    wifi_softap_->SaveConfig();

    system("killall hostapd");
    system("hostapd /tmp/hostapd.conf -B");
    restart_time_ms_ = utility::time::getms();

    std::ofstream o(network_config_file_);
    o << network_config_ << std::endl;
    o.close();
  }

}

