################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/Drivers/Flash/flash.c" \
"../src/Drivers/Flash/flash_c55_driver.c" \

C_SRCS += \
../src/Drivers/Flash/flash.c \
../src/Drivers/Flash/flash_c55_driver.c \

C_DEPS_QUOTED += \
"./src/Drivers/Flash/flash.d" \
"./src/Drivers/Flash/flash_c55_driver.d" \

OBJS_QUOTED += \
"./src/Drivers/Flash/flash.o" \
"./src/Drivers/Flash/flash_c55_driver.o" \

C_DEPS += \
./src/Drivers/Flash/flash.d \
./src/Drivers/Flash/flash_c55_driver.d \

OBJS_OS_FORMAT += \
./src/Drivers/Flash/flash.o \
./src/Drivers/Flash/flash_c55_driver.o \

OBJS += \
./src/Drivers/Flash/flash.o \
./src/Drivers/Flash/flash_c55_driver.o \


# Each subdirectory must supply rules for building sources it contributes
src/Drivers/Flash/flash.o: ../src/Drivers/Flash/flash.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Drivers/Flash/flash.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/Drivers/Flash/flash.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Drivers/Flash/flash_c55_driver.o: ../src/Drivers/Flash/flash_c55_driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/Drivers/Flash/flash_c55_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/Drivers/Flash/flash_c55_driver.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


