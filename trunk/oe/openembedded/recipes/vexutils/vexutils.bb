DESCRIPTION = "VexPro utilities"
SRCREV = "HEAD"
PV = "0.0+svn${SRCREV}"
PR = "r0"
DEPENDS = ""

SRC_URI = "svn://terkos.googlecode.com/svn/trunk;module=src;proto=http \
	file://terkos-lcd-menu \
	file://terkos-user-program-launcher \
	"
S = "${WORKDIR}/src"

PACKAGES = "vexutils"

do_install() {
	   rsync --exclude=.debug -av dist/ "${D}/"
	   install -d ${D}${sysconfdir}/init.d
	   install -m 0755 ${WORKDIR}/terkos-lcd-menu ${D}${sysconfdir}/init.d/
	   install -m 0755 ${WORKDIR}/terkos-user-program-launcher ${D}${sysconfdir}/init.d/
}

pkg_postinst() {
if test "x$D" != "x"; then
	exit 1
else
	update-rc.d terkos-lcd-menu defaults 30
	update-rc.d terkos-user-program-launcher defaults 30
fi
}

pkg_postrm() {
if test "x$D" != "x"; then
	exit 1
else
	${sysconfdir}/init.d/terkos-lcd-menu stop
	update-rc.d -f terkos-lcd-menu remove
	${sysconfdir}/init.d/terkos-user-program-launcher stop
	update-rc.d -f terkos-user-program-launcher remove
fi
}

FILES_${PN} = /opt /srv /etc
