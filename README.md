# XiaoPi
Make your Raspberry Pi Zero W as a home security camera.

### How to Use
If you just want to use camera functions without development, please check out the [wiki](https://github.com/sepfy/xiaopi/wiki) for more details.

### How to Develop
XiaoPi is be constructed by Yocto project. Compile whole prject by following instructions
```bash
git clone --recursive https://github.com/sepfy/xiaopi
cd xiaopi
source init-build-env.sh
bitbake xiaopi
```
It will take a few hours to build OS image. After building, you can find the OS image in tmp/deplay/images/raspberrypi0-wifi




