#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "hal/stream_receiver.h"

StreamReceiver::StreamReceiver(std::string fifo) {
  fd_ = -1;
  thread_id_ = -1;
  fifo_ = fifo;
  pthread_mutex_init(&mutex_, 0);
  stream_queue_.front = NULL;
  stream_queue_.back = NULL;
  key_frame_buf_data_ = nullptr;
  key_frame_buf_len_ = 0;
}

void StreamReceiver::SetKeyFrame(uint8_t *buf, size_t len) {

  if(key_frame_buf_data_ != nullptr)
    free(key_frame_buf_data_);

  key_frame_buf_data_ = (uint8_t*)malloc(len);
  memcpy(key_frame_buf_data_, buf, len);
  key_frame_buf_len_ = len;
}

void StreamReceiver::AddFrame(uint8_t *buf, size_t len) {

  if(fd_ < 0) return; 
  StreamData *stream_data = NULL;
  stream_data = (StreamData*)malloc(sizeof(StreamData));
  stream_data->size = len;
  stream_data->buffer = (uint8_t*)malloc(len*sizeof(uint8_t));
  memcpy(stream_data->buffer, buf, len);
  stream_data->next = NULL;

  pthread_mutex_lock(&mutex_);
  Enqueue(stream_data);
  pthread_mutex_unlock(&mutex_);
}

void StreamReceiver::Enqueue(StreamData *stream_data) {
  if(stream_queue_.front == NULL) {
    stream_queue_.front = stream_data;
    stream_queue_.back = stream_data;
  }
  else {
    stream_queue_.back->next = stream_data;
    stream_queue_.back = stream_data;
  }
}

StreamData* StreamReceiver::Dequeue() {
  StreamData *stream_data = stream_queue_.front;

  if(stream_queue_.front == stream_queue_.back) {
    stream_queue_.front = NULL;
    stream_queue_.back = NULL;
  }
  else {
    stream_queue_.front = stream_queue_.front->next;
  }

  return stream_data;
}

void StreamReceiver::Open() {

  mkfifo(fifo_.c_str(), 0666);
  if(pthread_create(&thread_id_, NULL, TransportThread, this) != 0) {
    perror("Create pthread failed\n");
    return;
  }

  if(pthread_detach(thread_id_) != 0) {
    perror("Detach pthread failed\n");
  }
}

void StreamReceiver::CleanQueue() {

  StreamData *stream_data = stream_queue_.front;
  StreamData *stream_data_tmp;
  while(stream_data != NULL) {
    stream_data_tmp = stream_data->next;
    ReleaseStreamData(stream_data);
    stream_data = stream_data_tmp;
  }

  stream_queue_.front = NULL;
  stream_queue_.back = NULL;
}

void StreamReceiver::ReleaseStreamData(StreamData *stream_data) {
  if(stream_data == NULL) return;
  if(stream_data->buffer != NULL) {
    free(stream_data->buffer);
    stream_data->buffer = NULL;
    stream_data->next = NULL;
  }
  free(stream_data);

}

void* StreamReceiver::TransportThread(void *context) {

  StreamReceiver *stream_receiver = (StreamReceiver*)context;
  while(1) {
    stream_receiver->fd_ = -1;
    stream_receiver->fd_ = open(stream_receiver->fifo_.c_str(), O_WRONLY);
    fprintf(stdout, "Open with client fd = %d\n", stream_receiver->fd_);
    stream_receiver->Transport();
  }
  stream_receiver->thread_id_ = -1;
  pthread_exit(NULL);
}

void StreamReceiver::Transport() {

  int ret = -1;

  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGPIPE);
  pthread_sigmask(SIG_SETMASK, &mask, NULL);
  AddFrame(key_frame_buf_data_, key_frame_buf_len_);
  while(fd_ > 0) {
    usleep(16000);

    pthread_mutex_lock(&mutex_);
    StreamData *stream_data = Dequeue();
    pthread_mutex_unlock(&mutex_);

    if(stream_data != NULL) {

#if DEBUG
    for(int i = 0; i < 8; ++i) {
      printf("0x%x ", stream_data->buffer[i]);
    }
    printf("\n");
#endif

      ret = write(fd_, stream_data->buffer, stream_data->size);
      ReleaseStreamData(stream_data);
      if(ret < 0) {
        perror("write error");
        close(fd_);
        fd_ = -1;
        CleanQueue();
        break;
      }

    }
  }

}
