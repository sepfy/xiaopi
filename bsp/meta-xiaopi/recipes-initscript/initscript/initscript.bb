SUMMARY = "Initial boot script"
DESCRIPTION = "Script to do any first boot init, started as a systemd service which removes itself once finished"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://S01modules \
    file://S10partition \
    file://S20network \
    file://S99watchdog \
"


do_compile () {
}

do_install () {
    install -d ${D}${sysconfdir}/rcS.d/
    install -m 0755 ${WORKDIR}/S* ${D}${sysconfdir}/rcS.d/

}
