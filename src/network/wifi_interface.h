#ifndef NETWORK_WIFI_INTERFACE_H_
#define NETWORK_WIFI_INTERFACE_H_

#include <pthread.h>
#include <wpa_ctrl.h>

#include "wifi_event_observer.h"

class WifiInterface {

 public:
  WifiInterface(const char *interface_path);
  int Init();
  int StartMonitor();
  static void* MonitorThread(void *context);

  void SendCommand(const char *cmd, size_t cmd_len, char *reply, size_t *reply_len); 
  int SendCommand(const char *cmd, size_t cmd_len, char *reply, size_t *reply_len, const char *check); 
  void Subscribe(WifiEventObserver *event_observer) { event_observer_ = event_observer; };
  void Unsubscribe() { event_observer_ = nullptr; }
  void Notify(char *buf, int len);

 private:
  struct wpa_ctrl *monitor_conn_ = nullptr;
  struct wpa_ctrl *ctrl_conn_ = nullptr;
  WifiEventObserver *event_observer_ = nullptr;
  pthread_t tid_ = -1;
  const char *interface_path_;
  void Monitor();
};

#endif // NETWORK_WIFI_INTERFACE_H_
