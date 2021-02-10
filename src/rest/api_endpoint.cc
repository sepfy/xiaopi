#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "api_endpoint.h"
#include "network_router.h"
#include "account_router.h"
#include "system_router.h"
#include "file_router.h"

void ApiEndpoint::Init(int port) {
 
  port_ = port;
  account_router_ = new AccountRouter();
  network_router_ = new NetworkRouter();
  system_router_ = new SystemRouter();
  file_router_ = new FileRouter();
  Pistache::Address addr(Pistache::Ipv4::any(), port_);
  http_endpoint_ = std::make_shared<Pistache::Http::Endpoint>(addr);
  auto opts = Pistache::Http::Endpoint::options().threads(1);
  http_endpoint_->init(opts);
  SetupRoutes();
}

bool ApiEndpoint::AuthCheck(const Pistache::Rest::Request& request) {
  std::string token = request.headers().getRaw("Authorization").value();
  //std::cout << token << std::endl;
  return SystemManager::GetInstance()->hmac() == token ? true : false;
}

void ApiEndpoint::Serve() {
  http_endpoint_->setHandler(router_.handler());
  printf("Serve in port: %d\n", port_);
  http_endpoint_->serve();
}

void ApiEndpoint::SetupRoutes() {

  Pistache::Rest::Routes::Get(router_, "/:/:", Pistache::Rest::Routes::bind(&FileRouter::Get, file_router_));
  Pistache::Rest::Routes::Get(router_, "/", Pistache::Rest::Routes::bind(&FileRouter::GetIndex, file_router_));

  Pistache::Rest::Routes::Put(router_, "/api/v1/system/remote/deviceKey", Pistache::Rest::Routes::bind(&SystemRouter::PutRemoteDeviceKey, system_router_));
  Pistache::Rest::Routes::Get(router_, "/api/v1/system/remote/deviceKey", Pistache::Rest::Routes::bind(&SystemRouter::GetRemoteDeviceKey, system_router_));

  Pistache::Rest::Routes::Put(router_, "/api/v1/system/remote/enable", Pistache::Rest::Routes::bind(&SystemRouter::PutRemoteEnable, system_router_));
  Pistache::Rest::Routes::Get(router_, "/api/v1/system/remote/enable", Pistache::Rest::Routes::bind(&SystemRouter::GetRemoteEnable, system_router_));

  Pistache::Rest::Routes::Get(router_, "/api/v1/system/remote/deviceCode", Pistache::Rest::Routes::bind(&SystemRouter::GetRemoteDeviceCode, system_router_));

  Pistache::Rest::Routes::Post(router_, "/api/v1/account/login", Pistache::Rest::Routes::bind(&AccountRouter::PostLogin, account_router_));
  Pistache::Rest::Routes::Put(router_, "/api/v1/account/:username", Pistache::Rest::Routes::bind(&AccountRouter::Put, account_router_));

  Pistache::Rest::Routes::Get(router_, "/api/v1/network/status", Pistache::Rest::Routes::bind(&NetworkRouter::GetNetworkStatus, network_router_));
  Pistache::Rest::Routes::Get(router_, "/api/v1/network/hotspot", Pistache::Rest::Routes::bind(&NetworkRouter::GetHotspot, network_router_));
  Pistache::Rest::Routes::Get(router_, "/api/v1/network/wireless/connection", Pistache::Rest::Routes::bind(&NetworkRouter::GetWirelessConnection, network_router_));
  Pistache::Rest::Routes::Post(router_, "/api/v1/network/wireless/connection", Pistache::Rest::Routes::bind(&NetworkRouter::PostWirelessConnection, network_router_));

  Pistache::Rest::Routes::Post(router_, "/api/v1/system/command/:cmd", Pistache::Rest::Routes::bind(&SystemRouter::PostCommand, system_router_));

}

