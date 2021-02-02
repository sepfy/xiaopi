#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utility/utility.h"
#include "wifi_softap.h"

void WifiSoftap::OnEvent(char *buf, int len) {

  char cmd[64] = {0};
  char reply[256] = {0};
  size_t reply_len;

  if(strstr(buf, WPA_EVENT_CONNECTED) != nullptr) {
    PLOGI("Station is connected. Disable softap");
    sprintf(cmd, "DISABLE");
    SendCommand(cmd, sizeof(cmd), reply, &reply_len);

  }
  else if(strstr(buf, WPA_EVENT_DISCONNECTED) != nullptr) {
    PLOGI("Station is disconnected. Enable softap");
    sprintf(cmd, "ENABLE");
    SendCommand(cmd, sizeof(cmd), reply, &reply_len);
  }

}


