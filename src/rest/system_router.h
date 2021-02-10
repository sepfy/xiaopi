#ifndef REST_SYSTEM_ROUTER_H_
#define REST_SYSTEM_ROUTER_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "system/system_manager.h"

class SystemRouter {
 public:
  SystemRouter();
  void PostCommand(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void PutRemoteEnable(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void PutRemoteDeviceKey(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void GetRemoteEnable(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void GetRemoteDeviceKey(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void GetRemoteDeviceCode(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
};

#endif // REST_SYSTEM_ROUTER_H_
