#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "rest/api_endpoint.h"
#include "utility/utility.h"
#include "rtc/rtc_agent.h"

#ifndef DEVEL
#include "hal/mmal_capturer.h"
#endif


void* RtcAgentStart(void *data) {
  RtcAgent *rtc_agent = (RtcAgent*)data;
  rtc_agent->Start();
  pthread_exit(NULL);
}

int main(void) {

  SystemManager *system_manager = SystemManager::GetInstance();
  NetworkManager *network_manager = NetworkManager::GetInstance();
  RtcAgent rtc_agent(system_manager->device_code(), system_manager->ReadConfig("deviceKey"));
  bool remote_enable = false;

#ifndef DEVEL
  MmalCapturer mmal_capturer;
  mmal_capturer.Start();
#endif

  network_manager->WifiMonitor();

  system_manager->ReadConfig("remoteEnable", &remote_enable);
  if(remote_enable == true) {
    pthread_t rtc_agent_tid;
    int ret = pthread_create(&rtc_agent_tid, NULL, RtcAgentStart, &rtc_agent);
    if(ret == 0) {
      pthread_detach(rtc_agent_tid);
    }
  }

  ApiEndpoint api_endpoint;
#ifdef DEVEL
  api_endpoint.Init(8080);
#else
  api_endpoint.Init(80);
#endif
  api_endpoint.Serve();
  return 0;
}
