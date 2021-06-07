#ifndef NETWORK_WIFI_SOFTAP_H_
#define NETWORK_WIFI_SOFTAP_H_

#include <stdint.h>

#include "wifi_interface.h"
#include "wifi_station.h"
#include "wifi_event_observer.h"

#define SOFTAP_CTRL_INTERFACE "/var/run/hostapd/wlan1"

class WifiSoftap : public WifiInterface {

 public:
  WifiSoftap();
  void OnEvent(char *buf, int len);
  void Disable();
  void Enable();
  bool UpdateConfig(std::string key, std::string value);
  void SaveConfig();
 private:
  std::string customized_config_;
};


#endif // NETWORK_WIFI_SOFTAP_H_
