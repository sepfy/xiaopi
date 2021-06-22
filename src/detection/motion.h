#ifndef DETECTION_MOTION_H_
#define DETECTION_MOTION_H_

#include <opencv2/opencv.hpp>

using namespace cv;

class Motion {
 public:
  Motion(int width, int height, float threshold);
  int Detect(uint8_t *inputs);
  float *GetFrame();
 private:
  Mat prev_frame_;  
  Mat output_frame_;
  float threshold_;
  int width_;
  int height_;
};

#endif 
