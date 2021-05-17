SUMMARY = "Interactive Connectivity Establishment library"
DESCRIPTION = "Libnice is an implementation of the IETF's draft Interactive Connectivity Establishment standard (ICE)."
HOMEPAGE = "http://nice.freedesktop.org/wiki/"
SRC_URI = "http://nice.freedesktop.org/releases/libnice-${PV}.tar.gz"
SRC_URI[md5sum] = "408482fa4bab7c6b884b0fb9ad57a038"
SRC_URI[sha256sum] = "5eabd25ba2b54e817699832826269241abaa1cf78f9b240d1435f936569273f4"

LICENSE = "LGPLv2.1 & MPLv1.1"
LIC_FILES_CHKSUM = "file://COPYING;md5=9c42325015702feda4f4d2f19a55b767 \
                    file://COPYING.LGPL;md5=2d5025d4aa3495befef8f17206a5b0a1 \
                    file://COPYING.MPL;md5=3c617710e51cdbe0fc00716f056dfb1a \
"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += "file://meson.patch"

DEPENDS = "glib-2.0 glib-2.0-native openssl"

inherit meson

