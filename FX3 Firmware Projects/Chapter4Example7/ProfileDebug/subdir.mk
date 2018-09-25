################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application.c \
../DebugConsole.c \
../StartUp.c \
../cyfxtx.c 

S_UPPER_SRCS += \
../cyfx_gcc_startup.S 

OBJS += \
./Application.o \
./DebugConsole.o \
./StartUp.o \
./cyfx_gcc_startup.o \
./cyfxtx.o 

C_DEPS += \
./Application.d \
./DebugConsole.d \
./StartUp.d \
./cyfxtx.d 

S_UPPER_DEPS += \
./cyfx_gcc_startup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -D__CYU3P_TX__=1 -DCYU3P_PROFILE_EN -I"C:\Program Files (x86)\Cypress\EZ-USB FX3 SDK\1.3\/firmware/u3p_firmware/inc" -I.. -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm926ej-s -mthumb-interwork -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -DCYU3P_PROFILE_EN -I"C:\Program Files (x86)\Cypress\EZ-USB FX3 SDK\1.3\/firmware/u3p_firmware/inc" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm926ej-s -mthumb-interwork -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


