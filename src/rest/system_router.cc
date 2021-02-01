#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "rest/api_endpoint.h"
#include "rest/system_router.h"
#include "system/system_manager.h"

SystemRouter::SystemRouter() {
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

