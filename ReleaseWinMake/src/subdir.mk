################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvdodec.c \
../src/lvdoenc.c \
../src/lvdomain.c 

O_SRCS += \
../src/lvdoenc.o \
../src/lvdomain.o 

OBJS += \
./src/lvdodec.o \
./src/lvdoenc.o \
./src/lvdomain.o 

C_DEPS += \
./src/lvdodec.d \
./src/lvdoenc.d \
./src/lvdomain.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


