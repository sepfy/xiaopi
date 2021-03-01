#ifndef MEDIA_RECORDER_H_
#define MEDIA_RECORDER_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
};
#endif



class Recorder {
 public:
  void Stop();
  int Start();
  int OpenInput();
  void CloseInput();  
 private:
  int Mux();
  void UpdateSinkName();
  bool recording_ = false;
  char sink_name_[64];
  AVFormatContext *input_format_context_ = NULL;
  AVRational input_time_base_;
  AVDictionary *opts_ = NULL;
  double calc_duration_;
  double duration_;
  int max_frame_num_ = 30*60*5;
  bool recording_status_ = false;
};


#endif // MEDIA_RECORDER_H_
