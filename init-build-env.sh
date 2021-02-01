#!/bin/bash

BSP_PATH=$(pwd)/bsp
cd $BSP_PATH
source poky/oe-init-build-env build1
cp $BSP_PATH/meta-piipcam/conf/local.conf.sample conf/local.conf
cp $BSP_PATH/meta-piipcam/conf/bblayers.conf.sample conf/bblayers.conf

cat << EOF >> conf/bblayers.conf
BBLAYERS ?= "
  $BSP_PATH/piipcam/bsp/poky/meta
  $BSP_PATH/piipcam/bsp/poky/meta-poky
  $BSP_PATH/piipcam/bsp/poky/meta-yocto-bsp
  $BSP_PATH/piipcam/bsp/meta-openembedded/meta-oe
  $BSP_PATH/piipcam/bsp/meta-openembedded/meta-networking
  $BSP_PATH/piipcam/bsp/meta-openembedded/meta-python
  $BSP_PATH/piipcam/bsp/meta-openembedded/meta-filesystems
  $BSP_PATH/piipcam/bsp/meta-raspberrypi
  $BSP_PATH/piipcam/bsp/meta-piipcam
  "
EOF
