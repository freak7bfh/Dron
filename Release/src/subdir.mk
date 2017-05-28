################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/delay.c \
../src/main.c \
../src/motors.c \
../src/rcc.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/usart.c 

OBJS += \
./src/adc.o \
./src/delay.o \
./src/main.o \
./src/motors.o \
./src/rcc.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/usart.o 

C_DEPS += \
./src/adc.d \
./src/delay.d \
./src/main.d \
./src/motors.d \
./src/rcc.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32F0 -DSTM32 -DSTM32F030K6Tx -DUSE_STDPERIPH_DRIVER -DSTM32F030 -I"C:/Users/Jarek/stm32_workspace/dron/inc" -I"C:/Users/Jarek/stm32_workspace/dron/CMSIS/core" -I"C:/Users/Jarek/stm32_workspace/dron/CMSIS/device" -I"C:/Users/Jarek/stm32_workspace/dron/StdPeriph_Driver/inc" -O0 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


