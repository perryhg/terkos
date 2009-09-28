DESCRIPTION = "Linux Driver for VexPro motor device"
RDEPENDS = "kernel (${KERNEL_VERSION})"
SRCREV = "HEAD"
PV = "0.0+svn${SRCREV}"
PR = "r0"
DEPENDS = "virtual/kernel"

PR = "r1"

SRC_URI = "svn://terkos.googlecode.com/svn/trunk;module=src;proto=http \
 "

S = "${WORKDIR}/src/module"

inherit module

do_compile() {
	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
	oe_runmake -C qemotor ${MODULE_MAKE_FLAGS} ${MAKE_TARGETS}
	oe_runmake -C qeinterrupt ${MODULE_MAKE_FLAGS} ${MAKE_TARGETS}
}

do_install() {	
	install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/
	install -m 0644 ${S}/qemotor/qemotor.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/
	install -m 0644 ${S}/qeinterrupt/qeinterrupt.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/
}

PACKAGES = "${PN}"

FILES_${PN} = "${base_libdir}/modules/"
