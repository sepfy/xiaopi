#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef DEVEL
#include "hal/mmal_capturer.h"
#endif

#include "system/system_manager.h"
#include "utility/utility.h"
#include "media/rtsp_server.h"

#ifndef RASPIOS
#include "rtc/rtc_agent.h"
#include "network/network_manager.h"
#include "rest/api_endpoint.h"

void* RtcAgentStart(void *data) {
  RtcAgent *rtc_agent = (RtcAgent*)data;
  rtc_agent->Start();
  pthread_exit(NULL);
}
#endif

void Stream() {
  RtspServer rtsp_server;
  rtsp_server.Stream();
}

void Manage() {

  SystemManager *system_manager = SystemManager::GetInstance();

#ifndef DEVEL
  MmalCapturer mmal_capturer;
  mmal_capturer.Start();
#endif

#ifndef RASPIOS
  NetworkManager *network_manager = NetworkManager::GetInstance();
  RtcAgent rtc_agent(system_manager->device_code(), system_manager->ReadConfig("deviceKey"));
  bool remote_enable = false;
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
#else
  while(1) {
    sleep(1);
  }
#endif


}



int main(void) {

  pid_t pid = fork();

  switch(pid) {
    case -1:
      perror("fork()");
      break;

    case 0:
      Stream();
      break;
        
    default:
      Manage();
  }

  return 0;
}
