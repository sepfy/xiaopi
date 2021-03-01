#ifndef HAL_STREAM_RECEIVER_H_
#define HAL_STREAM_RECEIVER_H_

#include <string>
#include <pthread.h>

typedef struct StreamData {

  size_t size;
  uint8_t *buffer;
  struct StreamData *next;

} StreamData;

typedef struct StreamQueue {

  StreamData *front;
  StreamData *back;
  size_t capacity;

} StreamQueue;

class StreamReceiver {

 public:
  StreamReceiver(std::string fifo);
  ~StreamReceiver() {}; 
  void AddFrame(uint8_t *buf, size_t len);
  void Open();
  static void* TransportThread(void *context);
  void SetKeyFrame(uint8_t *buf, size_t len);
 private:
  int fd_;
  std::string fifo_;
  StreamQueue stream_queue_;
  pthread_t thread_id_;
  pthread_mutex_t mutex_;
  int key_frame_buf_len_;
  uint8_t *key_frame_buf_data_;
  void Transport();
  void Enqueue(StreamData *stream_data);
  StreamData* Dequeue();
  void ReleaseStreamData(StreamData *stream_data);
  void CleanQueue();
};

#endif // HAL_STREAM_RECEIVER_H_
