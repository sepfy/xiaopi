#include <unistd.h>
#include <vector>

#include "utility/utility.h"
#include "detection/detection.h"

Detection::Detection(int width, int height) :
 motion_(width, height, 0.05), 
 face_detector_(128, 128, 0.7, 0.3) {

  width_ = width;
  height_ = height;
  tid_ = -1;
  is_empty_ = true;
}

void* Detection::DetectionThread(void *data) {
  Detection *detection = (Detection*)data;

  while(1) {
    if(!detection->is_empty_) {
      if(detection->motion_.Detect(detection->enqueued_buffer_)) {
        //PLOGI("%lld\n", utility::time::getms());
      	std::vector<Box> detection_boxes =
         detection->face_detector_.Inference(detection->motion_.GetFrame());
        //PLOGI("%lld\n", utility::time::getms());

	pthread_mutex_lock(&detection->mutex_);
        detection->detection_boxes_ = detection_boxes; 
	pthread_mutex_unlock(&detection->mutex_);
        //if(detection->detection_boxes_.size() > 0) {
        //  printf("detect!!!\n");
	//}
      }
      detection->is_empty_ = true;
    }
  }
}

void Detection::ShowBox(uint8_t *buffer, size_t size) {

  if(size < 1920*1080)
    return;
  pthread_mutex_lock(&mutex_);
  for(int i = 0; i < detection_boxes_.size(); i++) {

    int lx = (detection_boxes_[i].cx - detection_boxes_[i].w*0.5)*1920;
    int ly = (detection_boxes_[i].cy - detection_boxes_[i].h*0.5)*1080;
    int rx = (detection_boxes_[i].cx + detection_boxes_[i].w*0.5)*1920;
    int ry = (detection_boxes_[i].cy + detection_boxes_[i].h*0.5)*1080;

    lx = (lx > 2) ? lx : 2;
    ly = (ly > 2) ? ly : 2;
    rx = (rx < 1918) ? rx : 1918;
    ry = (ry < 1078) ? ry : 1078;

    for(int x = (lx - 2); x < (rx + 2); x++) {
      buffer[1920*(ly-2) + x] = 255;
      buffer[1920*(ly-1) + x] = 255;
      buffer[1920*ly + x] = 255;
      buffer[1920*(ly+1) + x] = 255;
      buffer[1920*(ly+2) + x] = 255;

      buffer[1920*(ry-2) + x] = 255;
      buffer[1920*(ry-1) + x] = 255;
      buffer[1920*ry + x] = 255;;
      buffer[1920*(ry+1) + x] = 255;
      buffer[1920*(ry+2) + x] = 255;

    }

    for(int y = ly; y < ry; y++) {
      buffer[1920*y + (lx-2)] = 255;
      buffer[1920*y + (lx-1)] = 255;
      buffer[1920*y + (lx)] = 255;
      buffer[1920*y + (lx+1)] = 255;
      buffer[1920*y + (lx+2)] = 255;

      buffer[1920*y + (rx-2)] = 255;
      buffer[1920*y + (rx-1)] = 255;
      buffer[1920*y + (rx)] = 255;
      buffer[1920*y + (rx+1)] = 255;
      buffer[1920*y + (rx+2)] = 255;
    }
  }
  pthread_mutex_unlock(&mutex_);

}

void Detection::Detect() {

  if(tid_ == -1) {
    printf("detection thread start\n");
    face_detector_.LoadModel("/opt/xiaopi/face_detection_front.tflite");
    int ret = pthread_create(&tid_, NULL, Detection::DetectionThread, this);
    if(ret == 0) {
      pthread_detach(tid_);
    }
  }

}

void Detection::Enqueue(uint8_t *buffer, size_t size) {

  if(is_empty_) {
//    printf("Enqueue buffer %d\n", size);
    // FIXME: Buffer size is not equal 1920*1080*1.5
    memcpy(enqueued_buffer_, buffer, 1920*1080*3/2);
    is_empty_ = false;
  }
}
