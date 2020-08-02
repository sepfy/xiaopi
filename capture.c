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

#include "bcm_host.h"
#include "interface/vcos/vcos.h"
#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_util.h"

#include "queue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

es_queue *g_es_queue;
int g_es_fd = 0;
const char *g_es_fifo_path = "/tmp/cam.264";  

#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#define VIDEO_FPS 30 
#define VIDEO_WIDTH 1920
#define VIDEO_HEIGHT 1080


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
    
} PORT_USERDATA;

PORT_USERDATA userdata;
MMAL_STATUS_T status;


void* write_es_queue_thread(void *data) {

    int ret;
    while(1) {
        char buf[64];
	if(!g_es_fd) continue;
//	printf("%d\n", g_es_fd);
        pthread_mutex_lock(&mutex);
	es_data *data = dequeue_es_data(g_es_queue);
	pthread_mutex_unlock(&mutex);

	if(data != NULL)
          ret = write(g_es_fd, data->buffer, data->size);
        if(ret < 0) {
          perror("write error");
        }
	free_es_data(data);
        usleep(33000);
    }
    

}



void sigpipe_handler(int unused) {
  g_es_fd = 0;
  mmal_port_pool_destroy(userdata.encoder_input_port, userdata.encoder_input_pool);
  mmal_port_pool_destroy(userdata.encoder_output_port, userdata.encoder_output_pool);
  mmal_port_pool_destroy(userdata.camera_video_port, userdata.camera_video_port_pool);
  mmal_component_destroy(userdata.encoder);
  mmal_component_destroy(userdata.camera);
  mmal_component_destroy(userdata.preview);
}


int fill_port_buffer(MMAL_PORT_T *port, MMAL_POOL_T *pool) {
  int q;
  int num = mmal_queue_length(pool->queue);

  for (q = 0; q < num; q++) {
    MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(pool->queue);
    if(!buffer) {
      fprintf(stderr, "Unable to get a required buffer %d from pool queue\n", q);
    }

    if(mmal_port_send_buffer(port, buffer) != MMAL_SUCCESS) {
      fprintf(stderr, "Unable to send a buffer to port (%d)\n", q);
    }
  }
}

static void camera_video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
  PORT_USERDATA *userdata = (PORT_USERDATA *) port->userdata;

  MMAL_BUFFER_HEADER_T *output_buffer = mmal_queue_get(userdata->encoder_input_pool->queue);
  if(output_buffer) {
	  
    mmal_buffer_header_mem_lock(buffer);
    memcpy(output_buffer->data, buffer->data, buffer->length);
    output_buffer->length = buffer->length;
    mmal_buffer_header_mem_unlock(buffer);
              
    if (mmal_port_send_buffer(userdata->encoder_input_port, output_buffer) != MMAL_SUCCESS) {
      fprintf(stderr, "ERROR: Unable to send buffer \n");
    }
  }
  mmal_buffer_header_release(buffer);

  if(port->is_enabled) {
    MMAL_STATUS_T status;

    MMAL_BUFFER_HEADER_T *new_buffer;
    MMAL_POOL_T *pool = userdata->camera_video_port_pool;
    new_buffer = mmal_queue_get(pool->queue);

    if (new_buffer) {
      status = mmal_port_send_buffer(port, new_buffer);
    }

    if (!new_buffer || status != MMAL_SUCCESS) {
      fprintf(stderr, "[%s]Unable to return a buffer to the video port\n", __func__);
    }
  }

}

static void encoder_input_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
    mmal_buffer_header_release(buffer);
}

static void encoder_output_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {

  MMAL_BUFFER_HEADER_T *new_buffer;
  PORT_USERDATA *userdata = (PORT_USERDATA *) port->userdata;
  MMAL_POOL_T *pool = userdata->encoder_output_pool;

  if(g_es_fd) {
    mmal_buffer_header_mem_lock(buffer);
    es_data *es_data = create_es_data(buffer->length, buffer->data);
    pthread_mutex_lock(&mutex);
    enqueue_es_data(g_es_queue, es_data);
    pthread_mutex_unlock(&mutex);
    mmal_buffer_header_mem_unlock(buffer);
  }

  mmal_buffer_header_release(buffer);
    
  if (port->is_enabled) {
    MMAL_STATUS_T status;

    new_buffer = mmal_queue_get(pool->queue);

    if(new_buffer) {
      status = mmal_port_send_buffer(port, new_buffer);
    }

    if (!new_buffer || status != MMAL_SUCCESS) {
      fprintf(stderr, "[%s]Unable to return a buffer to the video port\n", __func__);
    }
  }
}

int init_camera(PORT_USERDATA *userdata) {
    MMAL_STATUS_T status;
    MMAL_COMPONENT_T *camera = 0;
    MMAL_ES_FORMAT_T *format;
    MMAL_PORT_T * camera_preview_port;
    MMAL_PORT_T * camera_video_port;
    MMAL_PORT_T * camera_still_port;
    MMAL_POOL_T * camera_video_port_pool;

    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: create camera %x\n", status);
        return -1;
    }
    userdata->camera = camera;
    userdata->camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];

    camera_preview_port = camera->output[MMAL_CAMERA_PREVIEW_PORT];
    camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];
    camera_still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];

    {
        MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {
            { MMAL_PARAMETER_CAMERA_CONFIG, sizeof (cam_config)},
            .max_stills_w = 1280,
            .max_stills_h = 720,
            .stills_yuv422 = 0,
            .one_shot_stills = 1,
            .max_preview_video_w = VIDEO_WIDTH,
            .max_preview_video_h = VIDEO_HEIGHT,
            .num_preview_video_frames = 3,
            .stills_capture_circular_buffer_height = 0,
            .fast_preview_resume = 0,
            .use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
        };
        mmal_port_parameter_set(camera->control, &cam_config.hdr);
    }

    format = camera_preview_port->format;
    format->encoding = MMAL_ENCODING_OPAQUE;
    format->encoding_variant = MMAL_ENCODING_I420;
    format->es->video.width = VIDEO_WIDTH;
    format->es->video.height = VIDEO_HEIGHT;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = VIDEO_WIDTH;
    format->es->video.crop.height = VIDEO_HEIGHT;

    status = mmal_port_format_commit(camera_preview_port);

    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: camera viewfinder format couldn't be set\n");
        return -1;
    }

    // Setup camera video port format
    mmal_format_copy(camera_video_port->format, camera_preview_port->format);

    format = camera_video_port->format;
    format->encoding = MMAL_ENCODING_OPAQUE;// MMAL_ENCODING_OPAQUE;	// MMAL_ENCODING_I420;
    format->encoding_variant = MMAL_ENCODING_I420;
    format->es->video.width = VIDEO_WIDTH;
    format->es->video.height = VIDEO_HEIGHT;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = VIDEO_WIDTH;
    format->es->video.crop.height = VIDEO_HEIGHT;
    format->es->video.frame_rate.num = VIDEO_FPS;
    format->es->video.frame_rate.den = 1;

    camera_video_port->buffer_num = 2;
    camera_video_port->buffer_size = (format->es->video.width * format->es->video.height * 12 / 8 ) * camera_video_port->buffer_num;

    fprintf(stderr, "camera video buffer_size = %d\n", camera_video_port->buffer_size);
    fprintf(stderr, "camera video buffer_num = %d\n", camera_video_port->buffer_num);
    status = mmal_port_format_commit(camera_video_port);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to commit camera video port format (%u)\n", status);
        return -1;
    }

    camera_video_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(camera_video_port, camera_video_port->buffer_num, camera_video_port->buffer_size);
    userdata->camera_video_port_pool = camera_video_port_pool;
    camera_video_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;


    status = mmal_port_enable(camera_video_port, camera_video_buffer_callback);

    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to enable camera video port (%u)\n", status);
        return -1;
    }

    status = mmal_component_enable(camera);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to enable camera (%u)\n", status);
        return -1;
    }

    fill_port_buffer(userdata->camera_video_port, userdata->camera_video_port_pool);

    if (mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
        printf("%s: Failed to start capture\n", __func__);
    }

    fprintf(stderr, "camera created\n");
    return 0;
}

int init_encoder(PORT_USERDATA *userdata) {
    MMAL_STATUS_T status;
    MMAL_COMPONENT_T *encoder = 0;
    MMAL_PORT_T *preview_input_port = NULL;

    MMAL_PORT_T *encoder_input_port = NULL, *encoder_output_port = NULL;
    MMAL_POOL_T *encoder_input_port_pool;
    MMAL_POOL_T *encoder_output_port_pool;

    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_VIDEO_ENCODER, &encoder);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to create preview (%u)\n", status);
        return -1;
    }

    encoder_input_port = encoder->input[0];
    encoder_output_port = encoder->output[0];
    userdata->encoder_input_port = encoder_input_port;
    userdata->encoder_output_port = encoder_input_port;

    mmal_format_copy(encoder_input_port->format, userdata->camera_video_port->format);
    encoder_input_port->buffer_size = encoder_input_port->buffer_size_recommended;
    encoder_input_port->buffer_num = 2;

    mmal_format_copy(encoder_output_port->format, encoder_input_port->format);

    encoder_output_port->buffer_size = encoder_output_port->buffer_size_recommended;
    encoder_output_port->buffer_num = 2;
    // Commit the port changes to the input port 
    status = mmal_port_format_commit(encoder_input_port);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to commit encoder input port format (%u)\n", status);
        return -1;
    }

    // Only supporting H264 at the moment
    encoder_output_port->format->encoding = MMAL_ENCODING_H264;//MMAL_ENCODING_H264;	// MMAL_ENCODING_MJPEG
    encoder_output_port->format->bitrate = 2000000;

    encoder_output_port->buffer_size = encoder_output_port->buffer_size_recommended;

    if (encoder_output_port->buffer_size < encoder_output_port->buffer_size_min) {
        encoder_output_port->buffer_size = encoder_output_port->buffer_size_min;
    }

    encoder_output_port->buffer_num = encoder_output_port->buffer_num_recommended;

    if (encoder_output_port->buffer_num < encoder_output_port->buffer_num_min) {
        encoder_output_port->buffer_num = encoder_output_port->buffer_num_min;
    }

    // Commit the port changes to the output port    
    status = mmal_port_format_commit(encoder_output_port);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to commit encoder output port format (%u)\n", status);
        return -1;
    }

    fprintf(stderr, "encoder input buffer_size = %d\n", encoder_input_port->buffer_size);
    fprintf(stderr, "encoder input buffer_num = %d\n", encoder_input_port->buffer_num);

    fprintf(stderr, "encoder output buffer_size = %d\n", encoder_output_port->buffer_size);
    fprintf(stderr, "encoder output buffer_num = %d\n", encoder_output_port->buffer_num);

    encoder_input_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(encoder_input_port, encoder_input_port->buffer_num, encoder_input_port->buffer_size);
    userdata->encoder_input_pool = encoder_input_port_pool;
    encoder_input_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;
    status = mmal_port_enable(encoder_input_port, encoder_input_buffer_callback);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to enable encoder input port (%u)\n", status);
        return -1;
    }
    fprintf(stderr, "encoder input pool has been created\n");

    encoder_output_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(encoder_output_port, encoder_output_port->buffer_num, encoder_output_port->buffer_size);
    userdata->encoder_output_pool = encoder_output_port_pool;
    encoder_output_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;

    status = mmal_port_enable(encoder_output_port, encoder_output_buffer_callback);
    if (status != MMAL_SUCCESS) {
        fprintf(stderr, "Error: unable to enable encoder output port (%u)\n", status);
        return -1;
    }
    fprintf(stderr, "encoder output pool has been created\n");    

    fill_port_buffer(encoder_output_port, encoder_output_port_pool);

    fprintf(stderr, "encoder has been created\n");
    return 0;
}




int start_enqueue_buffer() {

  memset(&userdata, 0, sizeof (PORT_USERDATA));

  bcm_host_init();
    
  if (init_camera(&userdata)) {
      fprintf(stderr, "Failed to init camera %x\n", status);
      return -1;
  }

  if (init_encoder(&userdata)) {
      fprintf(stderr, "Failed to init encoder %x\n", status);
      return -1;
  }

  vcos_semaphore_create(&userdata.complete_semaphore, "mmal_opencv_video", 0);

}

int main(int argc, char** argv) {

  g_es_queue = create_es_queue();
  mkfifo(g_es_fifo_path, 0666); 

  sigaction(SIGPIPE, &(struct sigaction){sigpipe_handler}, NULL);
  pthread_t t;
  pthread_create(&t, NULL, write_es_queue_thread, NULL);


  while(1) {

    if(!g_es_fd) {
      g_es_fd = open(g_es_fifo_path, O_WRONLY); 
      printf("Connect to client %d\n", g_es_fd);
      start_enqueue_buffer();
    }


    sleep(1);
  }

  return 0;
}

