################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Nextion/NexHardware.c \
../Drivers/Nextion/NexObject.c 

OBJS += \
./Drivers/Nextion/NexHardware.o \
./Drivers/Nextion/NexObject.o 

C_DEPS += \
./Drivers/Nextion/NexHardware.d \
./Drivers/Nextion/NexObject.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Nextion/NexHardware.o: ../Drivers/Nextion/NexHardware.c Drivers/Nextion/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/BASIC_IO" -I"D:/Andri/WIdya Imersif/Firmware-test/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/SERIAL" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/SERIAL/USBSerial" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/HX710B" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/BASIC_IO" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/E32_915T30D" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Nextion/NexHardware.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Nextion/NexObject.o: ../Drivers/Nextion/NexObject.c Drivers/Nextion/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/BASIC_IO" -I"D:/Andri/WIdya Imersif/Firmware-test/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/SERIAL" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/SERIAL/USBSerial" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/HX710B" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/BASIC_IO" -I"D:/Andri/WIdya Imersif/Firmware-test/Drivers/E32_915T30D" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Nextion/NexObject.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
