require swt3.4-gtk_${PV}.bb

PR = "r0"

DEPENDS += "libhildon libhildonfm"

SRC_URI += "file://swt-hildon.patch;patch=1"
