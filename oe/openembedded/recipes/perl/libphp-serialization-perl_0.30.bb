DESCRIPTION = "The standard database interface module for Perl."
SECTION = "libs"
LICENSE = "Artistic|GPL"
PR = "r0"

SRC_URI = "http://search.cpan.org/CPAN/authors/id/B/BO/BOBTFISH/PHP-Serialization-0.30.tar.gz"

S = "${WORKDIR}/PHP-Serialization-${PV}"

inherit cpan
