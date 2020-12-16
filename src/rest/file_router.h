#ifndef REST_FILE_ROUTER_H_
#define REST_FILE_ROUTER_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

class FileRouter {

 public:
  void Get(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void GetIndex(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
 private:

};

#endif // REST_FILE_ROUTER_H_
