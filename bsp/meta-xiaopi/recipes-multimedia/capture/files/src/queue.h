#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct es_data es_data;

struct es_data {
  size_t size;
  uint8_t *buffer;
  es_data *next;
};
typedef struct es_queue es_queue;

struct es_queue {
  es_data *front;
  es_data *back;
};


es_data* create_es_data(size_t size, uint8_t *buffer) {

  es_data *data = (es_data*)malloc(sizeof(es_data));
  data->size = size;
  data->buffer = (uint8_t*)malloc(size*sizeof(uint8_t));
  memcpy(data->buffer, buffer, size);
  data->next = NULL;
  return data;
}

void free_es_data(es_data *data) {
  if(data == NULL) return;
  if(data->buffer != NULL) {
    free(data->buffer);
    data->buffer = NULL;
    data->next = NULL;
  }
  free(data);
}



es_queue* create_es_queue() {
  es_queue *queue = (es_queue*)malloc(sizeof(es_queue));
  queue->front = NULL;
  queue->back = NULL;
  return queue;
}

void free_es_queue(es_queue *queue) {
  
  es_data *data = queue->front;
  es_data *data_tmp;
  while(data != NULL) {
    data_tmp = data->next;
    free_es_data(data);
    data = data_tmp;
  }
  

}
void enqueue_es_data(es_queue *queue, es_data *data) {

  if(queue->front == NULL) {
    queue->front = data;
    queue->back = data;
  }
  else {
    queue->back->next = data;
    queue->back = data;
  }

}

es_data* dequeue_es_data(es_queue *queue) {

  es_data *data = queue->front;

  if(queue->front == queue->back) {
    queue->front = NULL;
    queue->back = NULL; 
  }
  else {
    queue->front = queue->front->next;
  }

  return data;
}

