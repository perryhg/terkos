DESCRIPTION = "The Data Test Program (dt) is a generic data test program used to verify proper \
operation of peripherals, file systems, device drivers, or any data stream supported by the \
operating system."
SECTION = "console/tests"
HOMEPAGE = "http://home.comcast.net/~SCSIguy/SCSI_FAQ/RMiller_Tools/dt.html"
LICENSE = "Public domain"

SRC_URI = "http://home.comcast.net/~SCSIguy/SCSI_FAQ/RMiller_Tools/ftp/dt/dt-source.tar.gz"
S = "${WORKDIR}/${PN}.d-WIP"

do_compile() {
	${MAKE} -f Makefile.linux
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 dt ${D}${bindir}
}
