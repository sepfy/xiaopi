#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "api_endpoint.h"
#include "network_router.h"
#include "system/system_manager.h"

NetworkRouter::NetworkRouter() {
  network_manager_ = NetworkManager::GetInstance();
}

void NetworkRouter::PostWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
  
  json j;
  std::string body;

  body = request.body();
  j = json::parse(body);

  std::string ssid = j["ssid"];
  std::string psk = j["psk"];
  std::string security = j["security"];

  network_manager_->ApplyWpaConfig(ssid, psk, security);
  response.send(Pistache::Http::Code::Ok, "");
}

void NetworkRouter::GetWirelessConnection(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  j["ipAddr"] = NetworkManager::GetInstance()->GetNetworkStatus();
  std::cout << j["ipAddr"] << std::endl;;
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void NetworkRouter::GetHotspot(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
   
  std::string res = network_manager_->Scan();
  response.send(Pistache::Http::Code::Ok, res);
}


