SUMMARY = "Camera capture"
DESCRIPTION = "Capture Camera video stream"
LICENSE = "CLOSED"

inherit systemd
DEPENDS = "userland"
RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "capture.service"
INSANE_SKIP_${PN} = "ldflags"

SRC_URI =  " \
    file://src \
    file://capture.service \
"
S = "${WORKDIR}/src"

do_compile () {
    oe_runmake 
}

do_install () {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/src/capture ${D}${bindir}
    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/capture.service ${D}${systemd_unitdir}/system
}


