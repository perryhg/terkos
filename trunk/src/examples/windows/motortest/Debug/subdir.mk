################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../motortest.cxx 

OBJS += \
./motortest.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: TerkOS C++ Compiler'
	/OpenEmbedded/arm-oe-linux-uclibcgnueabi/bin/g++.exe -I/OpenEmbedded/arm-oe-linux-uclibcgnueabi/usr/include -I/OpenEmbedded/arm-oe-linux-uclibcgnueabi/include -O0 -g3 -g -Wall -c -fmessage-length=0 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


