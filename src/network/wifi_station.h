#ifndef NETWORK_WIFI_STATION_H_
#define NETWORK_WIFI_STATION_H_

#include "wifi_interface.h"
#include "wifi_event_observer.h"

#define STATION_CTRL_INTERFACE "/var/run/wpa_supplicant/wlan0"

class WifiStation : public WifiInterface {

 public:
  WifiStation() : WifiInterface(STATION_CTRL_INTERFACE) {};

};


#endif // NETWORK_WIFI_STATION_H_
