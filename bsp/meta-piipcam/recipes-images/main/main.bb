SUMMARY = "Main program"
DESCRIPTION = "Handle main function"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://main \
    file://main.service \
    file://dist/ \
"

inherit systemd

RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "main.service"
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"
INSANE_SKIP_${PN}_append = "already-stripped"

do_compile () {
}

do_install () {

    install -d ${D}${sysconfdir}
    cp -r ${WORKDIR}/dist ${D}${sysconfdir}

    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/main ${D}${bindir}

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/main.service ${D}${systemd_unitdir}/system
}
