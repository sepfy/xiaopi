#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utility/utility.h"
#include "wifi_softap.h"

WifiSoftap::WifiSoftap() : WifiInterface(SOFTAP_CTRL_INTERFACE) {

  char cmd[32] = {0};
  size_t cmd_len = 0;
  char reply[8] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "ENABLE");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);

}

void WifiSoftap::OnEvent(char *buf, int len) {

  char cmd[64] = {0};
  char reply[256] = {0};
  size_t reply_len = sizeof(reply);

  if(strstr(buf, WPA_EVENT_CONNECTED) != nullptr && softap_status_ != kDisable) {
    is_disconnection_ = false;
    PLOGI("Station is connected. Disable softap");
    sprintf(cmd, "DISABLE");
    SendCommand(cmd, sizeof(cmd), reply, &reply_len);
    softap_status_ = kDisable;
  }

  if(strstr(buf, WPA_EVENT_DISCONNECTED) != nullptr) {
    is_disconnection_ = true;
    disconnection_time_ms_ = utility::time::getms();
  }

  if(softap_status_ != kEnable) {
    if((utility::time::getms() - disconnection_time_ms_) > 60*1000 && is_disconnection_) { 
      PLOGI("Station is disconnected. Enable softap");
      sprintf(cmd, "ENABLE");
      SendCommand(cmd, sizeof(cmd), reply, &reply_len);
      softap_status_ = kEnable;
    }
  }
}


