# piipcam

Make your Raspberry Pi Zero W as a 1080P IP Camera. 

### Prepare
Connect your Raspberry Pi Zero W with a camera.
![hw](https://github.com/sepfy/readme-image/blob/main/IMG_20201101_183305.jpg)

### Installation

Download and install image to your SD card. The procedures are same as [Raspberry Pi standard image](https://www.raspberrypi.org/downloads/).

Date | Image | Size | md5sum
---- | ---- | ---- | ----- 
2020-10-25 | [piipcam-raspberrypi0-wifi-20201025115230.rootfs.rpi-sdimg.zip](https://drive.google.com/file/d/1HjRGvfZ9ICKcg_ABvFLTY7WV570TCOGg/view?usp=sharing) | 63MB | 6791270c8d81c486e420c0f52102bde4


### Evaluation
1. Insert SD card and power on Raspberry Pi Zero.
2. Connect your PC Wi-Fi to Raspberry Pi Zero's Wi-FI hotspot. The default SSID/password is piipcam/12345678
![wifi](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%206.29.49%20PM.png)
3. Open VLC player and enter the network source "rtsp://192.168.1.6:8554/live".
![rtsp](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%206.47.59%20PM.png)
4. Click "Open". You will see the real-time camera image.
![preview](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%207.01.40%20PM.png)

### Others
Improve latancy: Open preferences of VLC. Find the Network caching in "Show All" -> "Inputs/Codecs". Change the value to 100. The latency of streaming will less than 500 ms.

![step1](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%206.49.03%20PM.png)
![step2](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%206.49.18%20PM.png)
![step3](https://github.com/sepfy/readme-image/blob/main/Screen%20Shot%202020-11-01%20at%206.49.59%20PM.png)


