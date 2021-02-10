#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "rest/api_endpoint.h"
#include "rest/system_router.h"
#include "system/system_manager.h"

SystemRouter::SystemRouter() {
}

void SystemRouter::PutRemoteDeviceKey(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  std::string body;

  body = request.body();
  j = json::parse(body);

  std::string device_key = j["deviceKey"];

  if(device_key == "") {
    response.send(Pistache::Http::Code::Not_Acceptable, "Device key is empty");
    return;
  }

  SystemManager::GetInstance()->UpdateConfig("deviceKey", device_key);
  response.send(Pistache::Http::Code::Ok, "");
}

void SystemRouter::PutRemoteEnable(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  std::string body;

  body = request.body();
  j = json::parse(body);
  std::cout << j << std::endl;
  SystemManager::GetInstance()->UpdateConfig("remoteEnable", (bool)j["remoteEnable"]);
  response.send(Pistache::Http::Code::Ok, "");
}

void SystemRouter::GetRemoteDeviceKey(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  j["deviceKey"] = SystemManager::GetInstance()->ReadConfig("deviceKey");
  std::cout << j.dump() << std::endl;
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void SystemRouter::GetRemoteEnable(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
  json j;
  bool value;
  SystemManager::GetInstance()->ReadConfig("remoteEnable", &value);
  j["remoteEnable"] = value;
  std::cout << j.dump() << std::endl;
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void SystemRouter::GetRemoteDeviceCode(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
  json j;
  j["deviceCode"] = SystemManager::GetInstance()->ReadConfig("deviceCode");
  std::cout << j.dump() << std::endl;
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void SystemRouter::PostCommand(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  std::string cmd = request.param(":cmd").as<std::string>();

  if(cmd == "reset") {
    SystemManager::GetInstance()->ResetToDefault(); 
    response.send(Pistache::Http::Code::Ok, "");
  }
  
  response.send(Pistache::Http::Code::Not_Found, "");
}

