#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include <vector>

using namespace tflite;

typedef struct Box {

  float confidence;

  float cx;
  float cy;
  float w;
  float h;

  float points[12];

} Box;

class FaceDetector {

 public:
  FaceDetector(int width, int height, float confidence_threshold, float iou_threshold);
  ~FaceDetector();
  int LoadModel(const char *model_path);
  std::vector<Box> Inference(float *inputs);
  
 private:
  std::vector<Box> GenerateAnchors(int input_width, int input_height, std::vector<int> strides);
  std::vector<Box> Postprocess(int width, int height, float threshold, std::vector<Box> anchor_boxes,
   float* detections, float *scores);

  int BoxConfidenceArgmax(std::vector<Box> boxes);
  std::vector<Box> NonMaxSupression(std::vector<Box> candidate_boxes);
 
  std::unique_ptr<Interpreter> interpreter_;
  std::vector<Box> anchor_boxes_;
  int width_;
  int height_;
  float iou_threshold_;
  float confidence_threshold_;
};

#endif // FACE_DETECTOR_H_
