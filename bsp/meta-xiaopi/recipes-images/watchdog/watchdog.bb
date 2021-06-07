SUMMARY = "watchdog"
DESCRIPTION = "watchdog"
LICENSE = "CLOSED"

SRC_URI =  " \
    file://init.service \
    file://src/ \
"

do_compile() {
  cd ${WORKDIR}/src
  make
}

do_install () {

    install -d ${D}${sysconfdir}
    install -m 0644 ${WORKDIR}/init.service ${D}${sysconfdir}
    install -d ${D}${bindir}
    install -m 0744 ${WORKDIR}/src/watchdog ${D}${bindir}
}
