#ifndef MEDIA_RECORDER_H_
#define MEDIA_RECORDER_H_

#include <pthread.h>
#include <mp4v2/mp4v2.h>

class Recorder {

 public:
  Recorder();
  int Open();
  void Close();  
  void Stop();
  void Start();
  static void* RecordingThread(void *context);
 private:
  MP4FileHandle mp4_file_handle_;
  MP4TrackId mp4_video_track_id_;
  bool do_recording_;
  pthread_t tid_;
#if 0
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
#endif
};


#endif // MEDIA_RECORDER_H_
