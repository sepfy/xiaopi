SUMMARY = "Pear WebRTC SDK"
HOMEPAGE = "https://github.com/sepfy/pear"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=c5d0a1d3d65be88f47f607250dbe821d"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
S = "${WORKDIR}/git" 

DEPENDS = "glib-2.0 libsrtp libnice librtp"
SRCREV = "1e91c3199dc925738a929743442e877b6386cfc5"
SRC_URI = "gitsm://github.com/sepfy/pear.git;protocol=https;branch=main"
SRC_URI += "file://version.patch \
            file://srtp.patch \
"

inherit cmake

