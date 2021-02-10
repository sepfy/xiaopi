SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += "file://wpa_supplicant.service \
            file://defconfig \
"
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"

PACKAGES_prepend += "libwpa-client "
FILES_libwpa-client = "${libdir}/libwpa_client.so"

do_install_append() {
    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/wpa_supplicant.service ${D}${systemd_unitdir}/system/
    install -d ${D}/${libdir}
    install -m 755 ${S}/wpa_supplicant/libwpa_client.so ${D}/${libdir}/
    install -d ${D}/${includedir}
    install -m 755 ${S}/src/common/wpa_ctrl.h ${D}/${includedir}
}
