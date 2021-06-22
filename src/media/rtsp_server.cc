#include "liveMedia/liveMedia.hh"
#include "BasicUsageEnvironment/BasicUsageEnvironment.hh"

#include "media/rtsp_server.h"


void RtspServer::announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
 char const* streamName, char const* inputFileName) {

  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "\n\"" << streamName << "\" stream, from the file \""
      << inputFileName << "\"\n";
  env << "Play this stream using the URL \"" << url << "\"\n";
  delete[] url;
}

int RtspServer::Stream() {

  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env_ = BasicUsageEnvironment::createNew(*scheduler);

  RTSPServer* rtspServer = RTSPServer::createNew(*env_, 8554, NULL);
  if (rtspServer == NULL) {
    *env_ << "Failed to create RTSP server: " << env_->getResultMsg() << "\n";
    exit(1);
  }

  char const* descriptionString = "xiaopi";

  char const* streamName = "live";
  char const* inputFileName = "/tmp/cam.264";
  ServerMediaSession* sms = ServerMediaSession::createNew(*env_, streamName, streamName,
      descriptionString);
  sms->addSubsession(H264VideoFileServerMediaSubsession
      ::createNew(*env_, inputFileName, true));
  rtspServer->addServerMediaSession(sms);

  announceStream(rtspServer, sms, streamName, inputFileName);

  env_->taskScheduler().doEventLoop();

  return 0;
}


