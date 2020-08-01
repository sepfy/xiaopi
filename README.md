# piipcam

Make your Raspberry Pi Zero W as a 1080P IP Camera. 

### How to compile
```bash
# git clone https://github.com/sepfy/piipcam.git
# cd piipcam
# make
```

### How to run
Please ensure that you aleady install the camera and enable it in rasp-config.
```bash
# ./capture &
# ./rtsp_server
```

### Architecture
This RTSP server is based on on-demand case in live555 project. live555 library accepts the Linux named fifo as a vieo source. So I use MMAL to read camra image as H264 raw data and pass it to a Linux named fifo.

### Evaluation with pre-built image

* Connect your Raspberry Pi Zero W with a camera.

* Download pre-built image. This image is based on Yocto project. If you have interesting on it, you can check this repository.

Date | Link
---- | ---
2020-08-11 | pi-cam-image

* Install image to your SD card. The procedures are same as the stardard of Raspberry Pi [image](https://www.raspberrypi.org/downloads/).

* Power on your Raspberry Pi Zero.

* Connect your PC or mobile phone to Raspberry Pi Zero's Wi-FI hotspot. The default SSID/password is piipcam/12345678

* Open VLC player and enter the network source "rtsp://192.168.1.6:8554/live". You will see the real-time camera image.

### Tricks

* Improve latancy. Open preferences of VLC. Find the Network caching in "Show All" -> "Inputs/Codecs". Change the value to 100. The latency of image will less than 500ms.
