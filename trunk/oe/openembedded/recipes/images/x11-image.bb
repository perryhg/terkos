#Angstrom X11 image

ANGSTROM_EXTRA_INSTALL ?= ""
XSERVER ?= "xserver-kdrive-fbdev"

export IMAGE_BASENAME = "x11-image"

DEPENDS = "task-base"
IMAGE_INSTALL = "\
    ${XSERVER} \
    task-base-extended \
    angstrom-x11-base-depends \
    angstrom-gpe-task-base \
    angstrom-gpe-task-settings \
    ${ANGSTROM_EXTRA_INSTALL}"

IMAGE_PREPROCESS_COMMAND = "create_etc_timestamp"

#zap root password for release images
ROOTFS_POSTPROCESS_COMMAND += '${@base_conditional("DISTRO_TYPE", "release", "zap_root_password; ", "",d)}'

inherit image
