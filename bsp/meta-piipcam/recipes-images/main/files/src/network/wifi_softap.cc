#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "wifi_softap.h"

void WifiSoftap::OnEvent(char *buf, int len) {

  char cmd[64] = {0};
  char reply[256] = {0};
  size_t reply_len;

  if(strstr(buf, WPA_EVENT_CONNECTED) != nullptr) {
    fprintf(stdout, "Station is connected. Disable softap\n");
    sprintf(cmd, "DISABLE");
    SendCommand(cmd, sizeof(cmd), reply, &reply_len);

  }
  else if(strstr(buf, WPA_EVENT_DISCONNECTED) != nullptr) {
    fprintf(stdout, "Station is disconnected. Enable softap\n");
    sprintf(cmd, "ENABLE");
    SendCommand(cmd, sizeof(cmd), reply, &reply_len);
  }

}


