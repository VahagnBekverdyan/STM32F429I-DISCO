################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/ts3510/ts3510.c 

OBJS += \
./Utilities/Components/ts3510/ts3510.o 

C_DEPS += \
./Utilities/Components/ts3510/ts3510.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/ts3510/%.o: ../Utilities/Components/ts3510/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429ZITx -DSTM32F429I_DISCO -DSTM32 -DSTM32F4 -DUSE_HAL_DRIVER -DSTM32F429xx Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/CMSIS/core" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/CMSIS/device" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/HAL_Driver/Inc/Legacy" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/HAL_Driver/Inc" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ampire480272" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ampire640480" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/Common" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/cs43l22" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/exc7200" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ft6x06" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ili9325" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ili9341" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/l3gd20" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/lis302dl" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/lis3dsh" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/lsm303dlhc" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/mfxstm32l152" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/n25q128a" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/n25q256a" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/n25q512a" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/otm8009a" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ov2640" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/s25fl512s" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/s5k5cag" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/st7735" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/stmpe1600" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/stmpe811" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/ts3510" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Components/wm8994" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Fonts" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/Log" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Utilities/STM32F429I-Discovery" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Middlewares/ST/STemWin/Config" Wilms/Documents/Ausbildung/Hochschule Darmstadt/5. Semester/Echtzeitprogrammierung/Labor/stm32f429i-disco_hal_lib/Middlewares/ST/STemWin/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<" @"Utilities/Components/ts3510/includes.args"
	@echo 'Finished building: $<'
	@echo ' '


