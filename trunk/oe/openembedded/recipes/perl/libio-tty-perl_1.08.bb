DESCRIPTION = "IO::Pty - Pseudo-TTY object class"
SECTION = "libs"
LICENSE = "Artistic|GPL"
HOMEPAGE = "http://search.cpan.org/~rgiersig/IO-Tty-${PV}/"
PR = "r0"

SRC_URI = "http://search.cpan.org/CPAN/authors/id/R/RG/RGIERSIG/IO-Tty-${PV}.tar.gz"

S = "${WORKDIR}/IO-Tty-${PV}"

inherit cpan
