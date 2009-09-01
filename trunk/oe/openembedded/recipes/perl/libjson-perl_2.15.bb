SECTION = "libs"
LICENSE = "Artistic|GPL"
HOMEPAGE = "http://search.cpan.org/~makamaka/JSON-2.15/lib/JSON.pm"
PR = "r1"

RDEPENDS = "perl-module-strict \
	 perl-module-base \
	 perl-module-overload \
	 perl-module-carp \
	 perl-module-carp-heavy \
	 perl-module-b \
	 perl-module-constant \
	 perl-module-bytes \
	 perl-module-encode \
	 perl-module-exporter \
	 perl-module-exporter-heavy \
	 perl-module-scalar-util \
	 perl-module-list-util \
	 perl-module-math-bigint \
	 perl-module-math-bigfloat \
	 perl-module-subs \
	 "

SRC_URI = "http://search.cpan.org/CPAN/authors/id/M/MA/MAKAMAKA/JSON-2.15.tar.gz"

S = "${WORKDIR}/JSON-${PV}"

EXTRA_CPANFLAGS = "EXPATLIBPATH=${STAGING_LIBDIR} EXPATINCPATH=${STAGING_INCDIR}"

inherit cpan
