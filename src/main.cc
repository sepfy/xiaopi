#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "rest/api_endpoint.h"
#include "utility/utility.h"

#ifndef DEVEL
#include "hal/mmal_capturer.h"
#endif

int main(void) {

  pid_t pid;
  pid = fork();

  if(pid == 0) {
#ifndef DEVEL
    MmalCapturer mmal_capturer;
    mmal_capturer.Start();
#endif
  }
  else if (pid > 0) {
    NetworkManager *network_manager = NetworkManager::GetInstance();
    network_manager->WifiMonitor();
    ApiEndpoint api_endpoint;
#ifdef DEVEL
    api_endpoint.Init(8080);
#else
    api_endpoint.Init(80);
#endif
    api_endpoint.Serve();
  }
  else {
    PLOGE("Forks error");
  }

  return 0;
}
