#include <algorithm>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "rest/api_endpoint.h"

int main(void) {

  NetworkManager *network_manager = NetworkManager::GetInstance();
  network_manager->WifiMonitor();

  ApiEndpoint api_endpoint;
#ifdef DEVEL
  api_endpoint.Init(8080);
#else
  api_endpoint.Init(80);
#endif
  api_endpoint.Serve();

  return 0;
}
