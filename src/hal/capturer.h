#ifndef HAL_CAPTURER_H_
#define HAL_CAPTURER_H_

#include <vector>
#include <iostream>

#include "hal/stream_receiver.h"

class Capturer {
 public:
  virtual void Start() = 0;
  void AddReceiver(std::string fifo);
  void Init();
  void Broadcast(uint8_t *buf, size_t len);
 private:
  std::vector<StreamReceiver> stream_receivers_;
  void CheckKeyFrame(uint8_t *buf, size_t len);
};

#endif // HAL_CAPTURER_H_
