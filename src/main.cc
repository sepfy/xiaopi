#include <time.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "network/network_manager.h"
#include "system/system_manager.h"
#include "rest/api_endpoint.h"
#include "utility/utility.h"
#include "media/recorder.h"
#include "hal/mmal_capturer.h"

void* capture(void* data) {

  Recorder recorder;
  struct stat st = {0};
  if(stat("/home/root/", &st) == -1) {
    mkdir("/home/root/", 0700);
  }

  while(1) {
    recorder.Start();
    sleep(1);
  }
  pthread_exit(NULL);
}

int main(void) {

  pid_t pid;
  pid = fork();

  if(pid == 0) {
    MmalCapturer mmal_capturer;
//    pthread_t t;
//    pthread_create(&t, NULL, capture, NULL);
    mmal_capturer.Start();

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
