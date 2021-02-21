#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <wpa_ctrl.h>

#include "utility/utility.h"
#include "wifi_interface.h"

WifiInterface::WifiInterface(const char *interface_path) {

  interface_path_ = interface_path;

  monitor_conn_ = wpa_ctrl_open(interface_path_);
  if(monitor_conn_ == nullptr) {
    PLOGE("Create monitor conn failed");
    exit(1);
  }

  ctrl_conn_ = wpa_ctrl_open(interface_path_);
  if(ctrl_conn_ == nullptr) {
    PLOGE("Create control conn failed");
    exit(1);
  }

  if(wpa_ctrl_attach(monitor_conn_) != 0) {
    PLOGE("Attach monitor conn failed");
    return;
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

  if(monitor_conn_ == nullptr) {
    PLOGE("Monitor conn does not init");
    return;
  }

  if((conn_fd = wpa_ctrl_get_fd(monitor_conn_)) < 0) {
    PLOGE("Monitor conn get failed");
    return;
  }

  PLOGI("Wifi interface %s start to monitor", interface_path_);
  while (monitor_conn_) {

    FD_ZERO(&rfds);
    FD_SET(conn_fd, &rfds);
    if(select(conn_fd + 1, &rfds, NULL, NULL, &tv) < 0) {
       PLOGE("Select error");
       break;
    }

    if(FD_ISSET(conn_fd, &rfds)) {
      len = sizeof(buf) - 1;
      ret = wpa_ctrl_recv(monitor_conn_, buf, &len);
      if(ret < 0) {
        PLOGE("wpa_ctrl_recv error");
        break;
      }
      buf[len] = '\0';
      //printf("%s\n", buf);
      Notify(buf, len);
    }
    sleep(1);
  }
  tid_ = -1;
}

int WifiInterface::StartMonitor() {

  int ret = -1;

  if((ret = pthread_create(&tid_, NULL, WifiInterface::MonitorThread, this)) != 0) {
    PLOGE("Monitor thread create failed");
    return ret;
  }

  if((ret = pthread_detach(tid_)) != 0) {
    PLOGE("Detach monitor thread failed");
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

int WifiInterface::SendCommand(const char *cmd, size_t cmd_len,
 char *reply, size_t *reply_len, const char *check) {

  if(ctrl_conn_ == nullptr) {
    PLOGE("control conn does not init");
    return -1;
  }

  if(wpa_ctrl_request(ctrl_conn_, cmd, cmd_len, reply, reply_len, NULL) != 0) {
    PLOGE("WPA request cmd %s failed", cmd);
  }

  if(strstr(reply, check) == NULL) {
    PLOGE("WPA requests failed. cmd = %s, reply = %s", cmd, reply);
    return -1;
  }
  return 0;
}

void WifiInterface::SendCommand(const char *cmd, size_t cmd_len, char *reply, size_t *reply_len) {

  if(ctrl_conn_ == nullptr) {
    PLOGE("control conn does not init");
    return;
  }

  if(wpa_ctrl_request(ctrl_conn_, cmd, cmd_len, reply, reply_len, NULL) != 0) {
    PLOGE("WPA request cmd %s failed", cmd);
  }
}
