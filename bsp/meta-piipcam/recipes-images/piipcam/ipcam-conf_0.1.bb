SUMMARY = "Configuration files"
DESCRIPTION = "Kernel modules, networking..."
LICENSE = "CLOSED"

SRC_URI =  " \
    file://ipcam.conf \
"

inherit systemd

do_compile() {
}

do_install () {
    install -d ${D}${sysconfdir}/modules-load.d/
    install -m 0755 ${WORKDIR}/ipcam.conf ${D}${sysconfdir}/modules-load.d/
}

