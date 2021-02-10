#ifndef NETWORK_NETWORK_MANAGER_H_
#define NETWORK_NETWORK_MANAGER_H_

#include <string>
#include <vector>
#include <pthread.h>

#include "wifi_station.h"
#include "wifi_softap.h"

typedef struct {
  std::string bssid;
  std::string ssid;
  std::string key_mgmt;
  std::string wpa_state;
  int freq;
} WpaStatus;

typedef struct NetworkInterface_ {

  std::string interface;
  std::string addr;
  std::string netmask;
  std::string ether;

} NetworkInterface;


typedef struct {
  int freq;
  int signal;
  std::string bssid;
  std::string flags;
  std::string ssid;
} WpaInfo; 

class NetworkManager {

 public:
  std::string GetInterfaces();
  void AddNetwork(std::string ssid, std::string psk, std::string security);
  std::vector<WpaInfo> Scan();
  WpaStatus GetWpaStatus(); 
  static NetworkManager* GetInstance();
  std::string GetNetworkStatus();
  std::string GetStationStatus();
  std::string GetSoftapStatus();
  std::string GetUsbEthernetStatus();
  std::string GetStationNetmask();
  std::string GetStationHwAddr();
  void WifiMonitor();
 private:
  WifiStation wifi_station_;
  WifiSoftap wifi_softap_;
  NetworkManager();
#ifdef DEVEL
  std::string interface_ = "wlx40b07656eff8";
#else
  std::string interface_ = "wlan0";
#endif

};

#endif // NETWORK_NETWORK_MANAGER_H_


