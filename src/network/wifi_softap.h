#ifndef NETWORK_WIFI_SOFTAP_H_
#define NETWORK_WIFI_SOFTAP_H_

#include "wifi_interface.h"
#include "wifi_event_observer.h"

#define SOFTAP_CTRL_INTERFACE "/var/run/hostapd/wlan1"

class WifiSoftap : public WifiInterface, public WifiEventObserver {

 public:
  WifiSoftap() : WifiInterface(SOFTAP_CTRL_INTERFACE) {};
  void OnEvent(char *buf, int len);

};


#endif // NETWORK_WIFI_SOFTAP_H_
