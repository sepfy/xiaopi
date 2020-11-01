SUMMARY = "RTSP server"
DESCRIPTION = "RTSP server for camera"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://rtsp-server \
    file://rtsp-server.service \
    file://capture \
    file://capture.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "rtsp-server.service capture.service"

do_compile () {
}

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/rtsp-server ${D}${bindir}
    install -m 0755 ${WORKDIR}/capture ${D}${bindir}

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/rtsp-server.service ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/capture.service ${D}${systemd_unitdir}/system
}

