#include <stdio.h>
#include <stdlib.h>
#include "wpa_ctrl.h"
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>

#include "wifi_interface.h"

WifiInterface::WifiInterface(const char *interface_path) {

  interface_path_ = interface_path;

  monitor_conn_ = wpa_ctrl_open(interface_path_);
  if(monitor_conn_ == nullptr) {
    fprintf(stderr, "Create monitor conn error\n");
  }

  ctrl_conn_ = wpa_ctrl_open(interface_path_);
  if(ctrl_conn_ == nullptr) {
    fprintf(stderr, "Create ctrl conn error\n");
  }

  if(wpa_ctrl_attach(monitor_conn_) != 0) {
    fprintf(stderr, "Monitor connection attach failed\n");
  }

}

void WifiInterface::Monitor() {

  int ret;
  int conn_fd = -1;
  fd_set rfds;

  char buf[256];
  size_t len = sizeof(buf) - 1;

  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  if((conn_fd = wpa_ctrl_get_fd(monitor_conn_)) < 0) {
    fprintf(stderr, "Monitor conn does not init\n");
    return;
  }
  
  fprintf(stdout, "Wifi station starts to monitor\n");
  while (monitor_conn_) {

    FD_ZERO(&rfds);
    FD_SET(conn_fd, &rfds);
    if(select(conn_fd + 1, &rfds, NULL, NULL, &tv) < 0) {
       perror("select");
       break;
    }

    if(FD_ISSET(conn_fd, &rfds)) {
      len = sizeof(buf) - 1;
      ret = wpa_ctrl_recv(monitor_conn_, buf, &len);
      if(ret < 0) {
        perror("wpa_ctrl_recv");
        break;
      }
      buf[len] = '\0';
      //printf("%s\n", buf);
      Notify(buf, len);
    }
  }
  tid_ = -1;
}

int WifiInterface::StartMonitor() {

  int ret = -1;

  if((ret = pthread_create(&tid_, NULL, WifiInterface::MonitorThread, this)) != 0) {
    fprintf(stderr, "Thread create failed\n");
    return ret;
  }

  if((ret = pthread_detach(tid_)) != 0) {
    fprintf(stderr, "Thread etach failed\n");
    return ret;
  }

  return ret;
}

void* WifiInterface::MonitorThread(void *context) {
  WifiInterface *wifi_interface = (WifiInterface*)context;
  wifi_interface->Monitor(); 
  pthread_exit(NULL);
}

void WifiInterface::Notify(char *buf, int len) {

  if(event_observer_ != nullptr) {
    event_observer_->OnEvent(buf, len);
  }
}

void WifiInterface::SendCommand(const char *cmd, size_t cmd_len, char *reply, size_t *reply_len) {

  if(ctrl_conn_ != nullptr) {
    wpa_ctrl_request(ctrl_conn_, cmd, cmd_len, reply, reply_len, NULL);
  }
}
