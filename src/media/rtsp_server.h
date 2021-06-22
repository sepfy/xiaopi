#ifndef MEDIA_RTSP_SERVER_H_
#define MEDIA_RTSP_SERVER_H_

#include "liveMedia/liveMedia.hh"
#include "BasicUsageEnvironment/BasicUsageEnvironment.hh"

class RtspServer {

 public:
  int Stream();

 private:  
  void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
   char const* streamName, char const* inputFileName);
  UsageEnvironment* env_;
};

#endif // MEDIA_RTSP_SERVER_H_
