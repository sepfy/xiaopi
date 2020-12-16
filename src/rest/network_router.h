#ifndef REST_NETWORK_ROUTER_H_
#define REST_NETWORK_ROUTER_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "network/network_manager.h"

class NetworkRouter {

 public:
  NetworkRouter();
  void GetWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void PostWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void GetHotspot(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
 private:
  NetworkManager *network_manager_ = nullptr;
};

#endif // REST_NETWORK_ROUTER_H_
