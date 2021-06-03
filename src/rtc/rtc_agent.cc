#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "h264_packetizer.h"
#include "h264_parser.h"
#include "rtc_agent.h"
#include "utility/utility.h"

const int kFifoMaxSize = 65535;

const std::string kServer = "https://xiaopi.me";
const std::string kBroker = "ssl://xiaopi.me:8883";

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
}

void connlost(void *context, char *cause) {
  PLOGI("CONNECTION LOST");
  RtcAgent *rtc_agent = (RtcAgent*)context;
  while(true) {
    int ret = rtc_agent->Connect();

    if(ret == 0)
      break;
    PLOGE("Connect to MQTT server failed. Retry...");
    sleep(10);
  }

}

int RtcAgent::OnMessage(void *context, char *topic, int len, MQTTClient_message *message) {

    RtcAgent *rtc_agent = (RtcAgent*)context;
    PLOGI("Subscribe <-- %s", topic);
    std::string offer_topic = "/" + rtc_agent->device_code_ + "/offer";
    if(rtc_agent->video_thread_running_ || !rtc_agent->peer_initilized_
     ||rtc_agent->offer_received_) {
      return -1;
    }

    if(strcmp(topic, offer_topic.c_str()) == 0) {
      char *remote_sdp = (char*)message->payload;
      peer_connection_set_remote_description(rtc_agent->peer_connection_, remote_sdp);
      rtc_agent->offer_received_ = true;
      rtc_agent->SendAnswer();
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic);
    return 0;
}
void RtcAgent::SendAnswer() {

  std::string answer_topic = "/" + device_code_ + "/answer";
  
  MQTTClient_deliveryToken token;
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  pubmsg.payload = (char*)ice_candidate_.c_str();
  pubmsg.payloadlen = ice_candidate_.length();
  pubmsg.qos = 1;
  pubmsg.retained = 0;
  MQTTClient_publishMessage(client_, answer_topic.c_str(), &pubmsg, &token);
  
  PLOGI("Publish --> %s", answer_topic.c_str());

}

void RtcAgent::OnIceConnectionStateChange(iceconnectionstate_t state, void *context) {

  if(state == FAILED) {
    PLOGI("Disconnect... Stop streaming");
    RtcAgent *rtc_agent = (RtcAgent*)context;
    rtc_agent->video_thread_running_ = false;
    rtc_agent->peer_initilized_ = false;
    rtc_agent->offer_received_ = false;
  }

}

void RtcAgent::OnIcecandidate(char *sdp, void *context) {

  RtcAgent *rtc_agent = (RtcAgent*)context;
  rtc_agent->ice_candidate_ = std::string(g_base64_encode((const unsigned char *)sdp, strlen(sdp)));
  rtc_agent->peer_initilized_ = true; 
  PLOGI("%s", rtc_agent->ice_candidate_.c_str());
}

void* RtcAgent::SendVideoThread(void *data) {

  PLOGI("Start to send media");
  RtcAgent *rtc_agent = (RtcAgent*)data;
  char fifo[] = "/tmp/record.264";
  static h264_frame_t sps_frame;
  static h264_frame_t pps_frame;
  int ret = 0;

  rtp_encode_context_t *rtp_encode_context;
  rtp_encode_context = create_rtp_encode_context(rtc_agent);

  int fd = -1;
  fd = open(fifo, O_RDONLY);
  if(fd < 0) {
    printf("Cannot open %s\n", fifo);
    pthread_exit(NULL);
  }

  uint8_t buf[1024*1024*5] = {0};
  struct h264_frame_t *h264_frame;
  h264_frame = (h264_frame_t*)malloc(sizeof(h264_frame_t));
  h264_frame->buf = buf;

  size_t size = 0;
  static unsigned long timestamp = 0;


  while(rtc_agent->video_thread_running_) {

    size = read(fd, buf, sizeof(buf));
    h264_frame->size = size;

   if(h264_frame->buf[4] == 0x27) {
      sps_frame.size = h264_frame->size;
      sps_frame.buf = (uint8_t*)malloc(h264_frame->size);
      memcpy(sps_frame.buf, h264_frame->buf, h264_frame->size);
    }
    else if(h264_frame->buf[4] == 0x68) {
      pps_frame.size = h264_frame->size;
      pps_frame.buf = (uint8_t*)malloc(h264_frame->size);
      memcpy(pps_frame.buf, h264_frame->buf, h264_frame->size);
    }
    else if(h264_frame->buf[4] == 0x25) {
      int size_tmp = h264_frame->size;
      uint8_t *buf_tmp = NULL;
      buf_tmp = (uint8_t*)malloc(size_tmp);
      memcpy(buf_tmp, h264_frame->buf, size_tmp);

      h264_frame->size = size_tmp + sps_frame.size;
      memcpy(h264_frame->buf, sps_frame.buf, sps_frame.size);
      memcpy(h264_frame->buf + sps_frame.size, buf_tmp, size_tmp);
      free(buf_tmp);
    }

    rtp_encode_frame(rtp_encode_context, h264_frame->buf, h264_frame->size, timestamp);
    timestamp += 33000;
  }
  PLOGI("End of send video thread\n");
  close(fd);
  rtc_agent->media_thread_id_ = -1; 
  rtc_agent->InitPeerConnection();
  pthread_exit(NULL);

}

void RtcAgent::OnTransportReady(void *context) {

  RtcAgent *rtc_agent = (RtcAgent*)context;
  if(((int)rtc_agent->media_thread_id_) == -1) {
    rtc_agent->video_thread_running_ = true;
    pthread_create(&rtc_agent->media_thread_id_, NULL, RtcAgent::SendVideoThread, context);
    pthread_detach(rtc_agent->media_thread_id_);
  }
}

bool RtcAgent::UpdateDeviceKey(std::string device_code,
 std::string old_device_key, std::string new_device_key) {

  std::string resource = kServer + "/api/v1/device/" + device_code;
  std::string device_info = "{\"oldDeviceKey\":\"" + old_device_key +
   "\", \"newDeviceKey\":\"" + new_device_key + "\"}";

  PLOGI("Put device %s with data %s\n", resource.c_str(), device_info.c_str());
  int ret = utility::Http::Put(resource.c_str(), device_info.c_str());
  PLOGI("return stats = %d", ret);
  if(ret == 200) {
    return true;
  }
  return false;
}

void RtcAgent::Start() {

  long ret = 0;
  std::string resource = kServer + "/api/v1/device";
  std::string device_info = "{\"deviceCode\":\"" + device_code_ +
   "\", \"deviceKey\":\"" + device_key_ + "\"}";

  PLOGI("Post device info = %s to %s\n", device_info.c_str(), resource.c_str());
  while(true) {
    ret = utility::Http::Post(resource.c_str(), device_info.c_str());
    if(ret == 200)
      break;
    PLOGE("Regist device info failed (%d). Retry...", ret);
    sleep(10);
  }

  PLOGI("Register successfully");

  ret = 1;
  while(true) {
    ret = Connect();

    if(ret == 0)
      break;
    PLOGE("Connect to MQTT server failed. Retry...");
    sleep(10);
  }

}

int RtcAgent::InitPeerConnection() {

  peer_connection_destroy(peer_connection_);
  peer_connection_ = peer_connection_create();
  peer_connection_set_on_icecandidate(peer_connection_, (void*)RtcAgent::OnIcecandidate, this);
  peer_connection_set_on_transport_ready(peer_connection_, (void*)RtcAgent::OnTransportReady, this);
  peer_connection_set_on_iceconnectionstatechange(peer_connection_,
   (void*)RtcAgent::OnIceConnectionStateChange, this);

  peer_connection_create_answer(peer_connection_);
  media_thread_id_ = -1;
  return 0;
}

int RtcAgent::Connect() {

  int ret = -1;
  std::string topic = "/" + device_code_ + "/offer";
  MQTTClient_create(&client_, kBroker.c_str(), "",
   MQTTCLIENT_PERSISTENCE_NONE, NULL);

  conn_opts_.keepAliveInterval = 20;
  conn_opts_.cleansession = 1;
  conn_opts_.username = device_code_.c_str();
  conn_opts_.password = device_key_.c_str();

  MQTTClient_setCallbacks(client_, this, connlost, RtcAgent::OnMessage, delivered);

  conn_opts_.ssl = &ssl_opts_;
  conn_opts_.ssl->trustStore = "/opt/xiaopi/ca/mqtts-ca.crt";
  conn_opts_.ssl->enableServerCertAuth = false; 
  conn_opts_.ssl->sslVersion = 3;
 
  if((ret = MQTTClient_connect(client_, &conn_opts_)) != MQTTCLIENT_SUCCESS) {
    PLOGE("Failed to connect, return code %d\n", ret);
    return -1;
  }

  InitPeerConnection();
  PLOGI("Subscribe: %s", topic.c_str());
  MQTTClient_subscribe(client_, topic.c_str(), 1/*QOS*/);
  return 0;
}

void RtcAgent::Disconnect() {

  MQTTClient_disconnect(client_, 10000);
  MQTTClient_destroy(&client_);
}

