# Root filesystem image for Qwerk 1.2 (16MB flash)

IMAGE_PREPROCESS_COMMAND = "create_etc_timestamp"

ANGSTROM_EXTRA_INSTALL ?= ""

DEPENDS = "task-base-extended \
	   "

IMAGE_INSTALL = "task-base-extended \
	   "

export IMAGE_BASENAME = "console-image"
IMAGE_LINGUAS = ""

inherit image

