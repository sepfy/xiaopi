#!/bin/bash

BSP_PATH=$(pwd)/bsp
cd $BSP_PATH
source poky/oe-init-build-env rpi-build
cp $BSP_PATH/meta-xiaopi/conf/local.conf.sample conf/local.conf
cp $BSP_PATH/meta-xiaopi/conf/bblayers.conf.sample conf/bblayers.conf

cat << EOF >> conf/bblayers.conf
BBLAYERS ?= " \
  $BSP_PATH/poky/meta \
  $BSP_PATH/poky/meta-poky \
  $BSP_PATH/poky/meta-yocto-bsp \
  $BSP_PATH/meta-openembedded/meta-oe \
  $BSP_PATH/meta-openembedded/meta-networking \
  $BSP_PATH/meta-openembedded/meta-python \
  $BSP_PATH/meta-openembedded/meta-filesystems \
  $BSP_PATH/meta-raspberrypi \
  $BSP_PATH/meta-xiaopi \
  "
EOF
