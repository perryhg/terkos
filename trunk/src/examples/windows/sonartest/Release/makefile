################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: sonartest

# Tool invocations
sonartest: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TerkOS C++ Linker'
	/OpenEmbedded/arm-oe-linux-uclibcgnueabi/bin/g++.exe -L/OpenEmbedded/arm-oe-linux-uclibcgnueabi/lib -L/OpenEmbedded/arm-oe-linux-uclibcgnueabi/usr/lib -o"sonartest" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(EXECUTABLES) sonartest
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
