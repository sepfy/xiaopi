SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += "file://dnsmasq.conf"

