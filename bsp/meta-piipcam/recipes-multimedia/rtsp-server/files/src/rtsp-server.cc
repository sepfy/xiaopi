#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

UsageEnvironment* env;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "\n\"" << streamName << "\" stream, from the file \""
      << inputFileName << "\"\n";
  env << "Play this stream using the URL \"" << url << "\"\n";
  delete[] url;
}

int main(int argc, char** argv) {
  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, NULL);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  char const* descriptionString = "piipcam";

  char const* streamName = "live";
  char const* inputFileName = "/tmp/cam.264";
  ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName,
      descriptionString);
  sms->addSubsession(H264VideoFileServerMediaSubsession
      ::createNew(*env, inputFileName, true));
  rtspServer->addServerMediaSession(sms);

  announceStream(rtspServer, sms, streamName, inputFileName);

  env->taskScheduler().doEventLoop();

  return 0;
}


