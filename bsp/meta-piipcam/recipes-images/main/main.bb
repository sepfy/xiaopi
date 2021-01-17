SUMMARY = "Main program"
DESCRIPTION = "Handle main function"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://main.service \
    file://dist/ \
    file://src \
    file://CMakeLists.txt \
"

inherit systemd cmake

S = "${WORKDIR}"
EXTRA_OECMAKE = ""

DEPENDS = "nlohmann-json pistache openssl"
RDEPENDS_${PN} += "bash"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "main.service"
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_AUTO_ENABLE_${PN} = "enable"

do_install_append () {

    install -d ${D}${sysconfdir}
    cp -r ${WORKDIR}/dist ${D}${sysconfdir}

    install -d ${D}${systemd_unitdir}/system/
    install -m 0644 ${WORKDIR}/main.service ${D}${systemd_unitdir}/system
}
