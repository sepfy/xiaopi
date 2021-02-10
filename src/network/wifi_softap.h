#ifndef NETWORK_WIFI_SOFTAP_H_
#define NETWORK_WIFI_SOFTAP_H_

#include <stdint.h>

#include "wifi_interface.h"
#include "wifi_event_observer.h"

#define SOFTAP_CTRL_INTERFACE "/var/run/hostapd/wlan1"

typedef enum {
  kEnable,
  kDisable,
} SoftapStatus;

class WifiSoftap : public WifiInterface, public WifiEventObserver {

 public:
  WifiSoftap();
  void OnEvent(char *buf, int len);

 private:
  uint64_t disconnection_time_ms_;
  bool is_disconnection_ = true;
  SoftapStatus softap_status_;
};


#endif // NETWORK_WIFI_SOFTAP_H_
