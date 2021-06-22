#include "face_detector.h"

using namespace tflite;

#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

FaceDetector::FaceDetector(int width, int height, float confidence_threshold, float iou_threshold) {
  width_ = width;
  height_ = height;
  confidence_threshold_ = confidence_threshold;
  iou_threshold_ = iou_threshold;
}

FaceDetector::~FaceDetector() {

}

int FaceDetector::LoadModel(const char *model_path) {

  std::unique_ptr<tflite::FlatBufferModel> model =
   tflite::FlatBufferModel::BuildFromFile(model_path);
  TFLITE_MINIMAL_CHECK(model != nullptr);

  tflite::ops::builtin::BuiltinOpResolver resolver;
  InterpreterBuilder builder(*model, resolver);
  builder(&interpreter_);
  TFLITE_MINIMAL_CHECK(interpreter_ != nullptr);

  std::vector<int> strides;
  strides.push_back(8);
  strides.push_back(16);
  strides.push_back(16);
  strides.push_back(16);

  anchor_boxes_ = GenerateAnchors(width_, height_, strides);
  TFLITE_MINIMAL_CHECK(interpreter_->AllocateTensors() == kTfLiteOk);
  return 0;
}

std::vector<Box> FaceDetector::Inference(float *inputs) {

  
  TfLiteTensor *input_tensor = interpreter_->tensor(interpreter_->inputs()[0]);
  memcpy(interpreter_->typed_input_tensor<float>(0), inputs, width_*height_*3*sizeof(float));

  TFLITE_MINIMAL_CHECK(interpreter_->Invoke() == kTfLiteOk);

  TfLiteTensor *box_tensor = interpreter_->tensor(interpreter_->outputs()[0]);
  TfLiteTensor *score_tensor = interpreter_->tensor(interpreter_->outputs()[1]);

  float threshold = log(confidence_threshold_/(1.0 - confidence_threshold_));

  std::vector<int> outputs = interpreter_->outputs();
  size_t size = box_tensor->bytes/sizeof(float);
  //printf("outputs = %d, %d\n", box_tensor->bytes/4, score_tensor->bytes/4);

  std::vector<Box> candidate_boxes = Postprocess(width_, height_, threshold, anchor_boxes_,
   interpreter_->typed_output_tensor<float>(0), interpreter_->typed_output_tensor<float>(1));

  std::vector<Box> detection_boxes = NonMaxSupression(candidate_boxes);

  return detection_boxes;
}

std::vector<Box> FaceDetector::GenerateAnchors(int input_width, int input_height, std::vector<int> strides) {

  std::vector<Box> anchor_boxes;
  float anchor_offset_x = 0.5;
  float anchor_offset_y = 0.5;

  for(size_t i = 0; i < strides.size(); i++) {
    int feature_map_width = input_width/strides[i];
    int feature_map_height = input_height/strides[i];

    for(int y = 0; y < feature_map_height; y++) {
      for(int x = 0; x < feature_map_width; x++) {
        float cx = ((float)x + anchor_offset_x)/(float)feature_map_width;
        float cy = ((float)y + anchor_offset_y)/(float)feature_map_height;
        float w = 1.0;
        float h = 1.0;
        Box anchor_box;
        anchor_box.w = w;
        anchor_box.h = h;
        anchor_box.cx = cx;
        anchor_box.cy = cy;
        anchor_boxes.push_back(anchor_box);
        anchor_boxes.push_back(anchor_box);
      }
    }
  }
  return anchor_boxes;
}

std::vector<Box> FaceDetector::Postprocess(int width, int height, float threshold, std::vector<Box> anchor_boxes,
 float* detections, float *scores) {

  std::vector<Box> candidate_boxes;

  for(size_t i = 0; i < anchor_boxes.size(); i++) {
    if(scores[i] < threshold)
      continue;

    int index = i*16;
    Box candidate_box;
    candidate_box.cx = detections[index]/(float)width + anchor_boxes[i].cx;
    candidate_box.cy = detections[index+1]/(float)height + anchor_boxes[i].cy;
    candidate_box.w = detections[index+2]/(float)width;
    candidate_box.h = detections[index+3]/(float)height;

    // keypoint
    float *key_points = &detections[index+4];
    for(int j = 0; j < 12; j+=2) {
      candidate_box.points[j] = key_points[+j]/(float)width + anchor_boxes[i].cx;
      candidate_box.points[j+1] = key_points[j+1]/(float)height + anchor_boxes[i].cy;
    }
    candidate_boxes.push_back(candidate_box);
  }
  return candidate_boxes;
}

int FaceDetector::BoxConfidenceArgmax(std::vector<Box> boxes) {

  float max_confidence = 0;
  int max_index = 0;
  for(size_t i = 0; i < boxes.size(); i++) {
    if(boxes[i].confidence > max_confidence) {
      max_confidence = boxes[i].confidence;
      max_index = i;
    }
  }
  return max_index;
}

std::vector<Box> FaceDetector::NonMaxSupression(std::vector<Box> candidate_boxes) {

  std::vector<Box> detection_boxes;
  while(candidate_boxes.size() > 0) {
    size_t max_index = BoxConfidenceArgmax(candidate_boxes);
    Box selected_box = candidate_boxes[max_index];
    candidate_boxes.erase(candidate_boxes.begin() + max_index);
    detection_boxes.push_back(selected_box);

    float selected_box_left = selected_box.cx - 0.5*selected_box.w;
    float selected_box_top = selected_box.cy - 0.5*selected_box.h;
    float selected_box_right = selected_box.cx + 0.5*selected_box.w;
    float selected_box_bottom = selected_box.cy + 0.5*selected_box.h;

    auto box = candidate_boxes.begin();

    while(box != candidate_boxes.end()) {

      float box_left = box->cx - 0.5*box->w;
      float box_top = box->cy - 0.5*box->h;
      float box_right = box->cx + 0.5*box->w;
      float box_bottom = box->cy + 0.5*box->h;

      float union_box_w = std::min(selected_box_right, box_right)
       - std::max(selected_box_left, box_left);
      float union_box_h = std::min(selected_box_bottom, box_bottom)
       - std::max(selected_box_top, box_top);
      float iou = (union_box_w*union_box_h)/(selected_box.w*selected_box.h + box->w*box->h
       - union_box_w*union_box_h);

      if(iou > iou_threshold_ && union_box_w > 0 && union_box_h > 0)
        box = candidate_boxes.erase(box);
      else
        ++box;
    }
  }

  return detection_boxes;
}

