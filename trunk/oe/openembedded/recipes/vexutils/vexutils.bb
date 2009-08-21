DESCRIPTION = "VexPro utilities"
SRCREV = "HEAD"
PV = "0.0+svn${SRCREV}"
PR = "r0"
DEPENDS = ""

SRC_URI = "svn://terkos.googlecode.com/svn/trunk;module=src;proto=http"
S = "${WORKDIR}/src"

PACKAGES = "vexutils"

do_install() {
	   rsync --exclude=.debug -av dist/ "${D}/"
}

FILES_${PN} = /opt /srv
