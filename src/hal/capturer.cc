#include <string.h>

#include "utility/utility.h"
#include "hal/capturer.h"

void Capturer::Init() {

  for(size_t i = 0; i < stream_receivers_.size(); ++i) {
    stream_receivers_[i].Open();
  }
}

void Capturer::CheckKeyFrame(uint8_t *buf, size_t len) {
  if((buf[4] & 0x1f) == 7) {
    PLOGI("Find H264 SPS/PPS NAL Unit");
    for(size_t i = 0; i < stream_receivers_.size(); ++i) {
      stream_receivers_[i].SetKeyFrame(buf, len);
    }
  }
}

void Capturer::AddReceiver(std::string fifo) {

  StreamReceiver stream_receiver(fifo);
  stream_receivers_.push_back(stream_receiver); 

}

void Capturer::Broadcast(uint8_t *buf, size_t len) {

  CheckKeyFrame(buf, len);
  for(size_t i = 0; i < stream_receivers_.size(); ++i) {
    stream_receivers_[i].AddFrame(buf, len);
  }
}
