#ifndef NETWORK_WIFI_EVENT_OBSERVER_H_
#define NETWORK_WIFI_EVENT_OBSERVER_H_
#include <iostream>

class WifiEventObserver {

 public:
  virtual void OnEvent(char *buf, int len) = 0;

};

#endif // NETWORK_WIFI_EVENT_OBSERVER_H_
