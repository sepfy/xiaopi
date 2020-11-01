SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

inherit core-image

IMAGE_INSTALL += "openssh hostapd dnsmasq userland initscript rtsp-server"

