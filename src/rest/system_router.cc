#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "utility/utility.h"
#include "rest/api_endpoint.h"
#include "rest/system_router.h"
#include "system/system_manager.h"
#include "rtc/rtc_agent.h"

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


  PLOGI("%s",j.dump().c_str());
  if(j["deviceKey"] != nullptr) {
    std::string device_key = j["deviceKey"];
    if(device_key.length() < 6 || device_key.length() > 12) {
      response.send(Pistache::Http::Code::Not_Acceptable, "Device key must be 6-12 characters");
      return;
    }

    int ret = RtcAgent::UpdateDeviceKey(SystemManager::GetInstance()->device_code(),
     SystemManager::GetInstance()->ReadConfig("deviceKey"), device_key);

    if(!ret) {
      response.send(Pistache::Http::Code::Internal_Server_Error, "Internal Error");
      return;

    }

    SystemManager::GetInstance()->UpdateConfig("deviceKey", device_key);
    response.send(Pistache::Http::Code::Ok, "");
    return;
  }

  response.send(Pistache::Http::Code::Not_Acceptable, "Device key is empty");
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
  SystemManager::GetInstance()->UpdateConfig("remoteEnable", (bool)j["remoteEnable"]);
  PLOGI("%s",j.dump().c_str());
  response.send(Pistache::Http::Code::Ok, "");
}

void SystemRouter::GetRemoteDeviceKey(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }

  json j;
  j["deviceKey"] = SystemManager::GetInstance()->ReadConfig("deviceKey");
  PLOGI("%s",j.dump().c_str());
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
  PLOGI("%s",j.dump().c_str());
  response.send(Pistache::Http::Code::Ok, j.dump());
}

void SystemRouter::GetRemoteDeviceCode(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return;
  }
  json j;
  j["deviceCode"] = SystemManager::GetInstance()->device_code();
  PLOGI("%s",j.dump().c_str());
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

  if(cmd == "reboot") {
    SystemManager::GetInstance()->Reboot(); 
    response.send(Pistache::Http::Code::Ok, "");
  }

  
  response.send(Pistache::Http::Code::Not_Found, "");
}

