#ifndef HAL_MMAL_CAPTURER_H_
#define HAL_MMAL_CAPTURER_H_

#include <iostream>
#include <memory>

#include "bcm_host.h"
#include "interface/vcos/vcos.h"
#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_util.h"

#include "detection/detection.h"
#include "hal/capturer.h"

typedef struct {
  MMAL_COMPONENT_T *camera;
  MMAL_COMPONENT_T *encoder;
  MMAL_COMPONENT_T *preview;

  MMAL_PORT_T *camera_video_port;
  MMAL_POOL_T *camera_video_port_pool;
  MMAL_PORT_T *encoder_input_port;
  MMAL_POOL_T *encoder_input_pool;
  MMAL_PORT_T *encoder_output_port;
  MMAL_POOL_T *encoder_output_pool;

  VCOS_SEMAPHORE_T complete_semaphore;

  void *mmal_capturer;

} PORT_USERDATA;



class MmalCapturer : public Capturer {
 public:
  void Start() override;

  std::shared_ptr<Detection> detection_;
 private:
  PORT_USERDATA userdata_;
  int InitMmal();
  int InitMmalCamera();
  int InitMmalEncoder();
};

#endif // HAL_MMAL_CAPTURER_H_
