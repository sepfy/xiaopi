SUMMARY = "Main program"
DESCRIPTION = "Handle main function"
LICENSE = "CLOSED"


FILESEXTRAPATHS_prepend := "${THISDIR}/../../../../:"

SRC_URI =  " \
    file://main.service \
    file://xiaopi \
    file://src/ \
    file://CMakeLists.txt \
"

inherit systemd cmake

S = "${WORKDIR}"
EXTRA_OECMAKE = ""

DEPENDS = "nlohmann-json pistache openssl wpa-supplicant userland pear libice libsrtp librtp paho-mqtt-c curl"
RDEPENDS_${PN} += "bash libwpa-client"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "main.service"
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"
INSANE_SKIP_${PN} = "ldflags"
FILES_${PN} += "/opt/xiaopi"

# -Wl,--as-needed cause that open pi camera failed
TARGET_LDFLAGS = "-Wl,-O1 -Wl,--hash-style=gnu"

do_install_append () {

    install -d ${D}/opt/
    cp -r ${WORKDIR}/xiaopi ${D}/opt/

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/main.service ${D}${systemd_unitdir}/system
}
