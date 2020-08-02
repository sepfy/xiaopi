LIVE555_INCLUDES = -I./live555/UsageEnvironment/include -I./live555/groupsock/include -I./live555/liveMedia/include -I./live555/BasicUsageEnvironment/include

LIVE555_LIBS = ./live555/liveMedia/libliveMedia.a ./live555/groupsock/libgroupsock.a ./live555/BasicUsageEnvironment/libBasicUsageEnvironment.a ./live555/UsageEnvironment/libUsageEnvironment.a

MMAL_INCLUDES = -I/opt/vc/include/ -I/opt/vc/include/interface/vmcs_host -I/opt/vc/include/interface/mmal/
MMAL_LIBS = -L/opt/vc/lib/ -lmmal_core -lmmal_util -lmmal_vc_client -lvcos -lbcm_host -lpthread


all: rtsp capture

rtsp: live555
	$(CXX) $(LIVE555_INCLUDES) rtsp_streamer.cc $(LIVEMEDIA_LIB) $(LIVE555_LIBS) -o rtsp-streamer


capture:
	$(CC) $(MMAL_INCLUDES) $(MMAL_LIBS) capture.c -o capture

.PHONY: live555
live555:
	cd live555 && ./genMakefiles linux && make -j4

clean:
	rm -rf capture rtsp-streamer
