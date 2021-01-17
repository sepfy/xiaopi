#ifndef NETWORK_NETWORK_MANAGER_H_
#define NETWORK_NETWORK_MANAGER_H_

#include <string>
#include <pthread.h>

typedef struct NetworkInterface_ {

  std::string interface;
  std::string addr;
  std::string netmask;
  std::string ether;

} NetworkInterface;

class NetworkManager {

 public:
  std::string GetInterfaces();
  bool ApplyWpaConfig(std::string ssid, std::string psk, std::string security);
  std::string Scan(); 
  static NetworkManager* GetInstance();
  std::string GetNetworkStatus();

 private:
  NetworkManager();
#ifdef DEVEL
  std::string interface_ = "wlx40b07656eff8";
#else
  std::string interface_ = "wlan0";
#endif

};

#endif // NETWORK_NETWORK_MANAGER_H_


