#ifndef NETWORK_WIFI_STATION_H_
#define NETWORK_WIFI_STATION_H_

#include <string>

#include "wifi_interface.h"
#include "wifi_event_observer.h"

#define STATION_CTRL_INTERFACE "/var/run/wpa_supplicant/wlan0"

class WifiStation : public WifiInterface {

 public:
  WifiStation();
  std::string GetScanResults();
  std::string GetStatus();
  void AddNetwork(std::string ssid, std::string psk, std::string security);
};


#endif // NETWORK_WIFI_STATION_H_
