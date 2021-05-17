DESCRIPTION = "librtp in media-server"
HOMEPAGE = "https://github.com/ireader/media-server"
SECTION = "libs"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=76e95d9ffdc1d2823cc6c2ae76af32ca"

S = "${WORKDIR}/git"
SRC_URI = "git://github.com/ireader/media-server.git;protocol=https"
SRCREV = "19d7d9831a7b8026434282630e76e87c585efbdb"
SRC_URI += "file://gccmk.patch"

do_configure() {
    echo "Do nothing"
}

do_compile() {
    make -C librtp
}

do_install() {
    install -d ${D}/${includedir}
    cp -rp ${S}/librtp/include/* ${D}/${includedir}/
    install -d ${D}/${libdir}
    install -m 644 ${S}/librtp/debug.linux/librtp.a ${D}/${libdir}
}
