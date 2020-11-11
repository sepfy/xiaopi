SUMMARY = "Configuration files"
DESCRIPTION = "Kernel modules, networking..."
LICENSE = "CLOSED"

SRC_URI =  " \
    file://ipcam.conf \
    file://hosts \
"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

do_install_append () {
    install -d ${D}${sysconfdir}/modules-load.d/
    install -m 0755 ${WORKDIR}/ipcam.conf ${D}${sysconfdir}/modules-load.d/
    install -m 0755 ${WORKDIR}/hosts ${D}${sysconfdir}/
}

