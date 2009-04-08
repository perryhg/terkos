#******************************************************************************
#
# Makefile - Rules for building a Linux system for the EDB9302.
#
# Copyright (c) 2004 Cirrus Logic, Inc.
#
#******************************************************************************
#
# Build RedBoot.
#
.PHONY: redboot
redboot:
	@BUILD_REDBOOT=1 sh build.sh


clean:
	@rm -rf .do_not_edit edb9302.log ramdisk.gz redboot.bin root opt zImage optfs.out
