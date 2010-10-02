require gdb-cross.inc

DEPENDS = "ncurses-sdk"
SRC_URI += " file://cygwin-deprecated.patch;patch=1 "

inherit sdk

PR = "r2"

do_stage() {
	:
}
