DESCRIPTION = "Expect - Expect for Perl"
SECTION = "libs"
LICENSE = "Artistic|GPL"
HOMEPAGE = "http://search.cpan.org/~rgiersig/Expect-${PV}/Expect.pod"
PR = "r0"

RDEPENDS = "perl-module-strict \
	 perl-module-base \
	 perl-module-overload \
	 perl-module-carp \
	 perl-module-exporter \
	 perl-module-exporter-heavy \
	 perl-module-posix \
	 perl-module-fcntl \
	 perl-module-io-handle \
	 perl-module-errno \
	 libio-tty-perl \
	 "

SRC_URI = "http://search.cpan.org/CPAN/authors/id/R/RG/RGIERSIG/Expect-${PV}.tar.gz"

S = "${WORKDIR}/Expect-${PV}"

inherit cpan
