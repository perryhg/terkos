require gdb-canadian-cross.inc

PR = "r1"

SRC_URI += " file://cygwin-deprecated.patch;patch=1 "

do_stage() {
	:
}
