require linux.inc

PR = "r2"

S = "${WORKDIR}/linux-2.6.29.2"

# Mark archs/machines that this kernel supports
DEFAULT_PREFERENCE = "-1"

SRC_URI = "${KERNELORG_MIRROR}/pub/linux/kernel/v2.6/linux-2.6.29.2.tar.bz2 \
           file://defconfig"

SRC_URI_append_qwerk = " \
	   file://edb93xx-audio.patch;patch=1 \
	   "
