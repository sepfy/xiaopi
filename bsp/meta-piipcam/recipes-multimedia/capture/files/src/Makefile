MMAL_LIBS = -lmmal_core -lmmal_util -lmmal_vc_client -lvcos -lbcm_host -lpthread

all:
	$(CC) $(LDFLAGS) capture.c $(MMAL_LIBS) -o capture

clean:
	rm -rf capture
