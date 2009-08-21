# Root filesystem image for VexPro (16MB flash)

IMAGE_PREPROCESS_COMMAND = "create_etc_timestamp"

ANGSTROM_EXTRA_INSTALL ?= ""

DEPENDS = " \
	task-boot \
	usbutils \
	wireless-tools \
	wpa-supplicant \
	alsa-utils \	
	fis \
	perl \
	openssh \
	gdbserver \
	   "

IMAGE_INSTALL = " \
	task-boot \
	kernel-module-sd-mod \
	kernel-module-scsi-mod \
	kernel-module-usb-storage \
	kernel-module-snd-usb-audio \
	kernel-module-snd-mixer-oss \
	kernel-module-snd-pcm-oss \
	kernel-module-uvcvideo \
	kernel-module-gspca-zc3xx \
	kernel-module-v4l1-compat \
	kernel-module-v4l2-common \
	kernel-module-videodev \
	kernel-module-zd1211rw \
	kernel-module-rt73usb \
	kernel-module-ecb \
	kernel-module-arc4 \
	kernel-module-aes \
	rt73-firmware \
	zd1211-firmware \
	fis \
	libstdc++ \
	openssh \
	openssh-sftp-server \
	gdbserver \
	busybox-httpd \
	usbutils \
	perl \
	libjson-perl \
	wireless-tools \
	wpa-supplicant \
	alsa-utils-alsactl \
	alsa-utils-amixer \
	alsa-utils-aplay \
	vexutils \
	   "

export IMAGE_BASENAME = "vexpro-image"
IMAGE_LINGUAS = ""

#ONLINE_PACKAGE_MANAGEMENT = "none"

inherit image
