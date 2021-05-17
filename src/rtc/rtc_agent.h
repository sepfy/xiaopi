#ifndef RTC_RTC_AGENT_H_
#define RTC_RTC_AGENT_H_

#include <pthread.h>
#include <string>

#include "MQTTClient.h"

extern "C"
{
#include "pear.h"
}


class RtcAgent {
 public:
  RtcAgent(std::string client_id) { client_id_ = client_id; }
  ~RtcAgent() {}
  int Connect();
  void Disconnect();
  static int OnMessage(void *context, char *topic, int len, MQTTClient_message *message);
  static void OnTransportReady(void *context);
  static void OnIcecandidate(char *sdp, void *context);
  static void* SendVideoThread(void *context);
  int InitPeerConnection();
 private:

  peer_connection_t peer_connection_;
  std::string client_id_;
  std::string ice_candidate_;
  MQTTClient client_;
  MQTTClient_connectOptions conn_opts_ = MQTTClient_connectOptions_initializer;
  //MQTTClient_message pubmsg = MQTTClient_message_initializer;
  //MQTTClient_deliveryToken token;
  void SendAnswer();
  pthread_t media_thread_id_;
};

#endif // RTC_RTC_AGENT_H_

