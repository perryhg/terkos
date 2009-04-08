#
# Redirect stdout and stderr to a file, and make descriptor 3 be stdout.  This
# will capture all output into the log file, and allow informational messages
# to be displayed during the build via "echo {blah} >&3".
#
exec 3>&1 > build.log 2>&1

#
# Get the rules for building the various packages.
#
BASEDIR=`pwd`
. ${BASEDIR}/buildrules

#
# If everything should be built, then set all the build flags.
#
if [ "${BUILD_ALL}" = "1" ]
then
    BUILD_RAMDISK=1
    BUILD_LINUX=1
    BUILD_REDBOOT=1
    BUILD_RW=1
    BUILD_FIRMWARE=1
    BUILD_ICE=1
fi

#
# See if the linux kernel should be configured.
#
if [ "${BUILD_LINUX_CONFIG}" = "1" ]
then
    #
    # Configure the linux kernel.
    #
    build_linux `pwd` `pwd`/linux.config config

    #
    # Success.
    #
    exit 0
fi

#
# See if busybox should be configured.
#
if [ "${BUILD_BUSYBOX_CONFIG}" = "1" ]
then
    #
    # Configure busybox.
    #
    build_busybox `pwd` `pwd`/busybox.config config

    #
    # Success.
    #
    exit 0
fi

#
# See if the ramdisk should be built.
#
if [ "${BUILD_RAMDISK}" = "1" ]
then
    #
    # Create the initial root filesystem.
    #
    echo Creating root filesystem... >&3
    try rm -rf root
    try mkdir -p root/dev
    try mkdir -p root/sys
    try mkdir -p root/etc
    try mkdir -p root/home/www
    try mkdir -p root/sbin
    try mkdir -p root/usr/bin
    try mkdir -p root/usr/share/udhcpc
    try ln -sf /proc/mounts root/etc/mtab
    try cp -dpR etc/* root/etc
    try cp `find sbin -type f -maxdepth 1 -print` root/sbin
    try cp `find udhcpc -type f -maxdepth 1 -print` root/usr/share/udhcpc
    try cp `find www -type f -maxdepth 1 -print` root/home/www

    #
    # Create the required symlinks in /dev.
    #
    create_standard_links `pwd`/root

    #
    # Build the kernel modules if necessary.
    #
    if [ "x`grep CONFIG_MODULES=y linux.config`" != "x" ]
    then
        build_linux `pwd`/root `pwd`/linux.config modules
    fi

    #
    # Build uclibc and add it to the root filesystem.
    #
    build_uclibc `pwd`/root 

    #
    # Build busybox and add it to the root filesystem.
    #
    build_module_tools `pwd`/root 
	
    #
    # Build busybox and add it to the root filesystem.
    #
    build_busybox `pwd`/root `pwd`/busybox.config

    #
    # Build Alsa-lib and Alsa-utils and add it to the root filesystem
    #

#removed because of the problem with building the .so files.  the src/libasound.la file generated from ltmain.sh has the wrong dlname and library_names.  they lack the .so in the filenames created.

    build_alsa `pwd`/root 

    #
    # Build gdbserver and add it to the root filesystem.
    #
    build_gdbserver `pwd`/root

    #
    # Build e2fsprogs and add it to the root filesystem.
    #
    build_e2fsprogs `pwd`/root mke2fs e2fsck.shared fsck

    #
    # Build dosfstools and add it to the root filesystem.
    #
    build_dosfstools `pwd`/root

    #
    # Build mtd and add it to the root filesystem.
    #
    build_mtd `pwd`/root

    #
    # Build the wlan-ng tools.
    #
    build_wlan_ng `pwd`/root

    build_wireless `pwd`/root

    #
    # Create a jffs2 image of the root filesystem
    #
    build_jffs2 `pwd`/root edb9302

    #
    # Generate a ramdisk image of the root filesystem.
    #
    build_ramdisk -d `pwd`/root       \
                  -b 8184             \
                  -i 1024              \
                  -r 0                \
                  -D `pwd`/device.txt \
                  -q                  \
                  `pwd`/ramdisk

    #
    # Compress the root filesystem.
    #
    echo Compressing ramdisk image... >&3
    try gzip -f -9 ramdisk
fi

#
# See if the linux kernel should be built.
#
if [ "${BUILD_LINUX}" = "1" ]
then
    #
    # Now that the ramdisk has been created, a bootpImage can be created if
    # required.
    #
    build_linux `pwd` `pwd`/linux.config zImage
fi

#
# See if RedBoot should be built.
#
if [ "${BUILD_REDBOOT}" = "1" ]
then
    #
    # Build RedBoot.
    #
    build_redboot `pwd` edb9302_ROMRAM
fi

#
# See if the read/write filesystem should be built
#


if [ "${BUILD_ICE}" = "1" ]
then
    echo Building IceE... >&3
    build_IceE `pwd`
fi

#if [ "${BUILD_FIRMWARE}" = "1" ]
#then
#    echo Building firmware... >&3
#    build_firmware `pwd`
#fi

#if [ "${BUILD_RW}" = "1" ]
#then
    #
    # Build optfs.out from opt
    #

   # echo Building r/w filesystem... >&3

   # try ./buildoptfs.sh
#fi

#
# Success.
#
exit 0
