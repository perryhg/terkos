################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/helloworld.c 

OBJS += \
./src/helloworld.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TerkOS C Compiler'
	/OpenEmbedded/arm-oe-linux-uclibcgnueabi/bin/gcc.exe -I/OpenEmbedded/arm-oe-linux-uclibcgnueabi/include -I/OpenEmbedded/arm-oe-linux-uclibcgnueabi/usr/include -O3 -Wall -c -fmessage-length=0 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


