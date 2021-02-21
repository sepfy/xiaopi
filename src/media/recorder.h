#ifndef MEDIA_RECORDER_H_
#define MEDIA_RECORDER_H_

class Recorder {
 public:
  void Stop();
  int Start();
  
 private:
  void UpdateSinkName();
  bool recording_ = false;
  char sink_name_[64];
};


#endif // MEDIA_RECORDER_H_
