DESCRIPTION = "VexPro utilities"
SRCREV = "HEAD"
PV = "0.0+svn${SRCREV}"
PR = "r0"
DEPENDS = ""

SRC_URI = "svn://terkos.googlecode.com/svn/trunk;module=src;rev=head;proto=http \
	file://terkos-init \
	file://terkos-lcd-menu \
	file://terkos-user-program-launcher \
	file://firmware.json.in \
	"
S = "${WORKDIR}/src"

PACKAGES = "vexutils"

do_install() {
	   rsync --exclude=.debug -av dist/ "${D}/"
	   install -d ${D}${sysconfdir}/init.d
	   install -m 0755 ${WORKDIR}/terkos-init ${D}${sysconfdir}/init.d/
	   install -m 0755 ${WORKDIR}/terkos-lcd-menu ${D}${sysconfdir}/init.d/
	   install -m 0755 ${WORKDIR}/terkos-user-program-launcher ${D}${sysconfdir}/init.d/
	   sed -e "s,__REVISION__,${SRCREV},g;s,__DATE__,`date +%Y-%m-%d\ %H:%M:%S`,g" \
	       < ${WORKDIR}/firmware.json.in > ${D}/opt/config/firmware.json
}

pkg_postinst() {
if test "x$D" != "x"; then
	exit 1
else
	update-rc.d terkos-init defaults 15 85
	update-rc.d terkos-lcd-menu defaults 30
	update-rc.d terkos-user-program-launcher defaults 30
fi
}

pkg_postrm() {
if test "x$D" != "x"; then
	exit 1
else
	${sysconfdir}/init.d/terkos-init stop
	update-rc.d -f terkos-init remove
	${sysconfdir}/init.d/terkos-lcd-menu stop
	update-rc.d -f terkos-lcd-menu remove
	${sysconfdir}/init.d/terkos-user-program-launcher stop
	update-rc.d -f terkos-user-program-launcher remove
fi
}

FILES_${PN} = /opt /srv /etc
