SUMMARY = "RTSP server"
DESCRIPTION = "RTSP server for camera"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://src \
    file://rtsp-server.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "rtsp-server.service"

DEPENDS = "live555"

S = "${WORKDIR}/src"

do_compile () {
    oe_runmake
}

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${S}/rtsp-server ${D}${bindir}

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/rtsp-server.service ${D}${systemd_unitdir}/system
}

