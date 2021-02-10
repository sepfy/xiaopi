#include <string>
#include <string.h>

#include "utility/utility.h"
#include "wifi_station.h"

WifiStation::WifiStation() : WifiInterface(STATION_CTRL_INTERFACE) {

  char cmd[32] = {0};
  size_t cmd_len = 0;
  char reply[8] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "ENABLE");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);

}

std::string WifiStation::GetStatus() {

  char cmd[128] = {0};
  size_t cmd_len = 0;
  char reply[1024] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "STATUS");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);

  std::string ret(reply);
  return ret;
}

void WifiStation::AddNetwork(std::string ssid, std::string psk, std::string security) {
  char cmd[128] = {0};
  size_t cmd_len = 0;
  char reply[128] = {0};
  size_t reply_len = sizeof(reply);

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "REMOVE_NETWORK all");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "ADD_NETWORK");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "0");

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "SET_NETWORK 0 ssid \"%s\"", ssid.c_str());
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "SET_NETWORK 0 key_mgmt %s", security.c_str());
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

  if(security != "None") {
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "SET_NETWORK 0 psk \"%s\"", psk.c_str());
    cmd_len = strlen(cmd);
    SendCommand(cmd, cmd_len, reply, &reply_len, "OK");
  }

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "SAVE_CONFIG");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "ENABLE_NETWORK all");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

}

std::string WifiStation::GetScanResults() {

  char cmd[128] = {0};
  size_t cmd_len;
  char reply[8192] = {0};
  size_t reply_len = sizeof(reply);
  
  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "SCAN");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len, "OK");

  reply_len = sizeof(reply);
  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "SCAN_RESULTS");
  cmd_len = strlen(cmd);
  SendCommand(cmd, cmd_len, reply, &reply_len);
  std::string ret(reply);
  return ret;
}

