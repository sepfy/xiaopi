#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <nlohmann/json.hpp>
#include "rest/account_router.h"
#include "system/system_manager.h"
#include "utility/security.h"
#include "rest/api_endpoint.h"

AccountRouter::AccountRouter() {
  system_manager_ = SystemManager::GetInstance(); 
}

void AccountRouter::PostLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  json j;
  std::string body;
	  
  body = request.body();
  j = json::parse(body);

  if(j["password"] == system_manager_->ReadConfig("password") &&
   j["username"] == system_manager_->ReadConfig("username")) {
    std::string hmac = system_manager_->hmac();
    response.send(Pistache::Http::Code::Ok, hmac);
  }
  else 
    response.send(Pistache::Http::Code::Unauthorized, "");
}


void AccountRouter::Put(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  if(!ApiEndpoint::AuthCheck(request)) {
    response.send(Pistache::Http::Code::Unauthorized, "");
    return; 
  }

  json j;
  std::string body;
	  
  body = request.body();
  j = json::parse(body);

  std::string password1, password2;
  password1 = j["password1"];
  password2 = j["password2"];

  if(password1 != password2) {
    response.send(Pistache::Http::Code::Not_Acceptable, "Password is not equal");
    return;
  }
  
  if(password1 == "") {
    response.send(Pistache::Http::Code::Not_Acceptable, "Password is empty");
    return;
  }

  if(password1.length() > 32) {
    response.send(Pistache::Http::Code::Not_Acceptable, "Password is too long");
    return;
  }

  system_manager_->UpdateConfig("password", password1);
  response.send(Pistache::Http::Code::Ok, "");

}
