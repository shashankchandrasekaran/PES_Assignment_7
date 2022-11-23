################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Audio_input.c \
../source/Audio_output.c \
../source/PES_Assignment_Musical_Notes.c \
../source/autocorrelate.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/systick.c 

O_SRCS += \
../source/fp_trig.o 

C_DEPS += \
./source/Audio_input.d \
./source/Audio_output.d \
./source/PES_Assignment_Musical_Notes.d \
./source/autocorrelate.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/systick.d 

OBJS += \
./source/Audio_input.o \
./source/Audio_output.o \
./source/PES_Assignment_Musical_Notes.o \
./source/autocorrelate.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/systick.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\board" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\source" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\drivers" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\CMSIS" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\utilities" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_7\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Audio_input.d ./source/Audio_input.o ./source/Audio_output.d ./source/Audio_output.o ./source/PES_Assignment_Musical_Notes.d ./source/PES_Assignment_Musical_Notes.o ./source/autocorrelate.d ./source/autocorrelate.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/systick.d ./source/systick.o

.PHONY: clean-source

