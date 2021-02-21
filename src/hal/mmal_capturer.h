#ifndef HAL_MMAL_CAPTURER_H_
#define HAL_MMAL_CAPTURER_H_

#include <iostream>

#include "hal/capturer.h"

class MmalCapturer : public Capturer {
 public:
  void Start() override;
};

#endif // HAL_MMAL_CAPTURER_H_
