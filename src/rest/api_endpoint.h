#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "network_router.h"
#include "account_router.h"
#include "file_router.h"
#include "system_router.h"
#include "system/system_manager.h"
#include "network/network_manager.h"
class ApiEndpoint {

 public:
  void Init(int port);
  void Serve();
  static bool AuthCheck(const Pistache::Rest::Request& request);
 private:
  int port_;
  Pistache::Rest::Router router_;
  std::shared_ptr<Pistache::Http::Endpoint> http_endpoint_;
  NetworkRouter *network_router_ = nullptr;
  AccountRouter *account_router_ = nullptr;
  SystemRouter *system_router_ = nullptr;
  FileRouter *file_router_ = nullptr;
  void SetupRoutes();
};


