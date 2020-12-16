#ifndef REST_ACCOUNT_ROUTER_H_
#define REST_ACCOUNT_ROUTER_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "system/system_manager.h"

class AccountRouter {

 public:
  AccountRouter();
  void PostLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void Put(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

 private:
  SystemManager *system_manager_ = nullptr;

};

#endif // REST_ACCOUNT_ROUTER_H_
