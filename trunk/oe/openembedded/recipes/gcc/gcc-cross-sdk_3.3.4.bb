PR = "r7"

inherit sdk

require gcc-${PV}.inc
require gcc-cross-sdk.inc
require gcc-configure-sdk.inc
require gcc-package-sdk.inc

SRC_URI += 'file://sdk-libstdc++-includes.patch;patch=1'
