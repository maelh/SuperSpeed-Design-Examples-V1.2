################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cyfxbulklpauto.c \
../cyfxbulklpdscr.c \
../cyfxtx.c 

S_UPPER_SRCS += \
../cyfx_gcc_startup.S 

OBJS += \
./cyfx_gcc_startup.o \
./cyfxbulklpauto.o \
./cyfxbulklpdscr.o \
./cyfxtx.o 

C_DEPS += \
./cyfxbulklpauto.d \
./cyfxbulklpdscr.d \
./cyfxtx.d 

S_UPPER_DEPS += \
./cyfx_gcc_startup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\Cypress\FX3 SDK_1_1\\firmware\u3p_firmware\inc" -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm926ej-s -mthumb-interwork -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -D__CYU3P_TX__=1 -I"C:\Cypress\FX3 SDK_1_1\\firmware\u3p_firmware\inc" -I"..\." -Os -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm926ej-s -mthumb-interwork -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


