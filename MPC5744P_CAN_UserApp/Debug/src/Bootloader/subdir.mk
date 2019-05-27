################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/Bootloader/Bootloader.c" \
"../src/Bootloader/UserAppFlash.c" \

C_SRCS += \
../src/Bootloader/Bootloader.c \
../src/Bootloader/UserAppFlash.c \

C_DEPS_QUOTED += \
"./src/Bootloader/Bootloader.d" \
"./src/Bootloader/UserAppFlash.d" \

OBJS_QUOTED += \
"./src/Bootloader/Bootloader.o" \
"./src/Bootloader/UserAppFlash.o" \

C_DEPS += \
./src/Bootloader/Bootloader.d \
./src/Bootloader/UserAppFlash.d \

OBJS_OS_FORMAT += \
./src/Bootloader/Bootloader.o \
./src/Bootloader/UserAppFlash.o \

OBJS += \
./src/Bootloader/Bootloader.o \
./src/Bootloader/UserAppFlash.o \


# Each subdirectory must supply rules for building sources it contributes
src/Bootloader/Bootloader.o: ../src/Bootloader/Bootloader.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Bootloader/Bootloader.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/Bootloader/Bootloader.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Bootloader/UserAppFlash.o: ../src/Bootloader/UserAppFlash.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Bootloader/UserAppFlash.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/Bootloader/UserAppFlash.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


