################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\FastIO.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\I2CIO.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LCD.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_I2C.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_I2C_ByVac.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SI2C.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR1W.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR2W.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR3W.cpp \
C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\SI2CIO.cpp 

LINK_OBJ += \
.\libraries\NewliquidCrystal\FastIO.cpp.o \
.\libraries\NewliquidCrystal\I2CIO.cpp.o \
.\libraries\NewliquidCrystal\LCD.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_I2C.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_I2C_ByVac.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_SI2C.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_SR.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_SR1W.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_SR2W.cpp.o \
.\libraries\NewliquidCrystal\LiquidCrystal_SR3W.cpp.o \
.\libraries\NewliquidCrystal\SI2CIO.cpp.o 

CPP_DEPS += \
.\libraries\NewliquidCrystal\FastIO.cpp.d \
.\libraries\NewliquidCrystal\I2CIO.cpp.d \
.\libraries\NewliquidCrystal\LCD.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_I2C.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_I2C_ByVac.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_SI2C.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_SR.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_SR1W.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_SR2W.cpp.d \
.\libraries\NewliquidCrystal\LiquidCrystal_SR3W.cpp.d \
.\libraries\NewliquidCrystal\SI2CIO.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\NewliquidCrystal\FastIO.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\FastIO.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\I2CIO.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\I2CIO.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LCD.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LCD.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_I2C.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_I2C.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_I2C_ByVac.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_I2C_ByVac.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_SI2C.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SI2C.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_SR.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_SR1W.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR1W.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_SR2W.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR2W.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\LiquidCrystal_SR3W.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\LiquidCrystal_SR3W.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NewliquidCrystal\SI2CIO.cpp.o: C:\Program\ Files\ (x86)\Arduino\libraries\NewliquidCrystal\SI2CIO.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\eightanaloginputs" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire" -I"C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal" -I"C:\Program Files (x86)\Arduino\libraries\NewliquidCrystal\utility" -I"C:\Users\zick\eclipse\cpp-oxygen\eclipse\arduinoPlugin\libraries\LiquidCrystal_I2C\1.1.2" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


