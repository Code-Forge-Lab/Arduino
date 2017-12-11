################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2\LiquidCrystal_I2C.cpp 

O_SRCS += \
C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2\LiquidCrystal_I2C.o 

LINK_OBJ += \
.\libraries\LiquidCrystal_I2C\LiquidCrystal_I2C.cpp.o 

CPP_DEPS += \
.\libraries\LiquidCrystal_I2C\LiquidCrystal_I2C.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\LiquidCrystal_I2C\LiquidCrystal_I2C.cpp.o: C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2\LiquidCrystal_I2C.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


