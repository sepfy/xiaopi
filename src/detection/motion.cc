#include <sys/time.h>
#include "utility/utility.h"
#include "motion.h"

using namespace cv;

Motion::Motion(int width, int height, float threshold) {
  threshold_ = threshold;
  width_ = width;
  height_ = height;
}

float* Motion::GetFrame() {

  return (float*)output_frame_.data;
}

int Motion::Detect(uint8_t *inputs) {

  int ret = 0;
  struct timeval current_tv;
  gettimeofday(&current_tv, NULL);
  static struct timeval motion_tv = current_tv;

  if(current_tv.tv_sec - motion_tv.tv_sec < 1)
    return ret;

  static Mat prev_frame;
  Mat delta_frame;
  Mat current_frame = Mat(Size(1920, 1080*3/2), CV_8UC1, inputs);
  cvtColor(current_frame, current_frame, CV_YUV2RGB_I420);
  resize(current_frame, current_frame, Size(128, 128), 0, 0, INTER_NEAREST);
#if 0
  if(prev_frame.size().width == 128) {

    absdiff(prev_frame, current_frame, delta_frame);
    // 128*128*255
    float diff = (float)cv::sum(delta_frame)[0]/(12533760.0);
    if(diff > threshold_) {
      printf("motion!!\n");
      ret = 1;
      motion_tv = current_tv;

    }
  }
  current_frame.copyTo(prev_frame);
#endif
  current_frame.copyTo(output_frame_);
  output_frame_.convertTo(output_frame_, CV_32FC3);
  output_frame_ = (output_frame_ - 127.5)/127.5;
  ret = 1;
  return ret;
#if 0
static int count = 0;

PLOGI();
if(count %300==0) {
PLOGI();
FILE *fp = NULL;
fp = fopen("buffer.raw", "wb");
if(fp != NULL) {
	printf("write data\n");
  fwrite(current_frame.data, 128*128*3, 1, fp);
  fclose(fp);
}
}
count++;
#endif
}
