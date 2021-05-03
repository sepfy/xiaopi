#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "file_router.h"

void FileRouter::Get(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {

  std::string file_name = request.resource();

  std::size_t found = file_name.find("svg");
  if(found != std::string::npos) {
    Pistache::Http::serveFile(response, "/opt/xiaopi/dist/" + file_name, 
     Pistache::Http::Mime::MediaType::fromString("image/svg+xml"));
  }
  else {
    Pistache::Http::serveFile(response, "/opt/xiaopi/dist/" + file_name);
  }
}


void FileRouter::GetIndex(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Pistache::Http::serveFile(response, "/opt/xiaopi/dist/index.html");
}
