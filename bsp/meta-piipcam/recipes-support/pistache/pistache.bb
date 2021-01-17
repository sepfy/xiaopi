SUMMARY = "A high-performance REST Toolkit written in C++ "
HOMEPAGE = "https://github.com/pistacheio/pistache"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=fa818a259cbed7ce8bc2a22d35a464fc"

S = "${WORKDIR}/git" 
SRC_URI = "git://github.com/pistacheio/pistache.git;protocol=https"
 
SRCREV = "8f278dbd345b818740f865623d5039e9314352fd"
 
inherit cmake
