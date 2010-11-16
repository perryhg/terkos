require libtool.inc
require libtool_${PV}.bb

PR = "r1"
FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/libtool-${PV}"
SRC_URI_append = "\
 file://libtool-2.2.6a-1.src.patch \
"

S = "${WORKDIR}/libtool-2.2.6"
EXTRA_OECONF = " --disable-shared "

inherit sdk

do_install () {
	install -d ${D}${bindir}/
	install -m 0755 libtool ${D}${bindir}/
	install -m 0755 libtoolize ${D}${bindir}/

	install -d ${D}${libdir}/
	if [ `uname -o` = "Cygwin" ]; then
	    cp ${S}/libltdl/.libs/*.lib ${D}${libdir}
	else
	    oe_libinstall -a -so -C libltdl libltdl ${D}${libdir}
	fi

	install -d ${D}${includedir}/
	install -m 0644 libltdl/ltdl.h ${D}${includedir}

	install -d ${D}${datadir}/libtool/config/
	install -c ${S}/libltdl/config/config.guess ${D}${datadir}/libtool/
	install -c ${S}/libltdl/config/config.sub ${D}${datadir}/libtool/
	install -c -m 0644 ${S}/libltdl/config/ltmain.sh ${D}${datadir}/libtool/config/

	install -d ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/libtool.m4 ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltdl.m4 ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltoptions.m4 ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltversion.m4 ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltsugar.m4 ${D}${datadir}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/lt~obsolete.m4 ${D}${datadir}/aclocal/
}

do_stage () {
	install -d ${STAGING_BINDIR}/
	install -m 0755 libtool ${STAGING_BINDIR}/${HOST_SYS}-libtool
	install -m 0755 libtoolize ${STAGING_BINDIR}/libtoolize

	if [ `uname -o` = "Cygwin" ]; then
	    cp ${S}/libltdl/.libs/*.lib ${STAGING_LIBDIR}
	else
	    oe_libinstall -a -so -C libltdl libltdl ${STAGING_LIBDIR}
	fi
	install -d ${STAGING_INCDIR}
	install -m 0644 libltdl/ltdl.h ${STAGING_INCDIR}/

	install -d ${STAGING_DATADIR}/libtool/config/
	install -c ${S}/libltdl/config/config.guess ${STAGING_DATADIR}/libtool/
	install -c ${S}/libltdl/config/config.sub ${STAGING_DATADIR}/libtool/
	install -c -m 0644 ${S}/libltdl/config/ltmain.sh ${STAGING_DATADIR}/libtool/config/

	install -d ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/libtool.m4 ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltdl.m4 ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltoptions.m4 ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltversion.m4 ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/ltsugar.m4 ${STAGING_DATADIR}/aclocal/
	install -c -m 0644 ${S}/libltdl/m4/lt~obsolete.m4 ${STAGING_DATADIR}/aclocal/
}
