PR = "r19"

SRC_URI = "${GNU_MIRROR}/ncurses/ncurses-${PV}.tar.gz \
	file://ncurses-5.5-cygwin.patch;patch=1 \
	"
S = "${WORKDIR}/ncurses-${PV}"

require ncurses.inc

LEAD_SONAME = "libncurses.so.5"

# Do NOT use the one from autotools.bbclass, because running
# autoreconf in ncurses is not recommended.
do_configure() {
	if [ -e ${S}/configure ]; then
		oe_runconf $@
	else
		oenote "nothing to configure"
	fi
}