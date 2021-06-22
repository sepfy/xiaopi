#ifndef DETECTION_DETECTION_H_
#define DETECTION_DETECTION_H_

#include <stdint.h>
#include <pthread.h>
#include <queue>

#include "detection/face_detector.h"
#include "detection/motion.h"

class Detection {

 public:
  Detection(int width, int height);
  void Enqueue(uint8_t *buffer, size_t size);
  void Detect();
  void ShowBox(uint8_t *buffer, size_t size);
  static void* DetectionThread(void *context);
 private:
  Motion motion_;
  FaceDetector face_detector_;
  int width_;
  int height_;
  uint8_t enqueued_buffer_[1920*1080*3];
  pthread_t tid_;
  bool is_empty_;
  std::vector<Box> detection_boxes_;
  pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;  
};

#endif // DETECTION_DETECTION_H_
