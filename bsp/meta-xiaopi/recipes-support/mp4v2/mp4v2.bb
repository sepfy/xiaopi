SUMMARY = "MP4v2 Library"
HOMEPAGE = "https://code.google.com/archive/p/mp4v2"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=eb3014b036b6d2151d944aef6a84c36f"

S = "${WORKDIR}/git" 
SRC_URI = "git://github.com/TechSmith/mp4v2.git;protocol=https"
SRCREV = "master" 

inherit cmake

do_configure() {
    mkdir -p ${S}/cmake
    cd ${S}/cmake
    cmake ..
}

do_compile() {
    cd ${S}/cmake
    make -j4
}

do_install() {
    install -d ${D}/${includedir}
    cp -rp ${S}/include/* ${D}/${includedir}/
    install -d ${D}/${libdir}
    install -m 644 ${S}/cmake/libmp4v2.a ${D}/${libdir}
}
