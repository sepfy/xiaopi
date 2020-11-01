SUMMARY = "Initial boot script"
DESCRIPTION = "Script to do any first boot init, started as a systemd service which removes itself once finished"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://rc.local \
    file://initscript.service \
"

inherit systemd

RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "initscript.service"

do_compile () {
}

do_install () {
    install -d ${D}${sysconfdir}
    install -m 0755 ${WORKDIR}/rc.local ${D}${sysconfdir}

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/initscript.service ${D}${systemd_unitdir}/system
}
