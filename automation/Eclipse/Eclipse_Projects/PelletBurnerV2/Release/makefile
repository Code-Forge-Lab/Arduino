################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ..\makefile.init

RM := del 

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include versions\2.42v\subdir.mk
-include versions\2.421v_stable\subdir.mk
-include versions\2.4215v_stable\ 2017_12_11MovetoEclipse\subdir.mk
-include versions\2.4214v\subdir.mk
-include versions\2.4213v_stable\subdir.mk
-include versions\2.40v\subdir.mk
-include versions\2.39_stable_final\subdir.mk
-include versions\2.31v\subdir.mk
-include versions\2.30v\subdir.mk
-include versions\2.29v\subdir.mk
-include versions\2.21v\subdir.mk
-include versions\2.1v\subdir.mk
-include versions\2.0v\subdir.mk
-include versions\1.35v\subdir.mk
-include versions\1.2v\subdir.mk
-include versions\1.27v\subdir.mk
-include versions\1.24v\subdir.mk
-include versions\1.17v\subdir.mk
-include versions\1.15v\subdir.mk
-include versions\1.10v\subdir.mk
-include versions\1.0v\subdir.mk
-include libraries\Wire\src\utility\subdir.mk
-include libraries\Wire\src\subdir.mk
-include libraries\NewliquidCrystal\subdir.mk
-include libraries\LiquidCrystal_I2C\subdir.mk
-include core\core\subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ..\makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
C:\Users\zick\Documents\Arduino\Arduino\automation\Eclipse\Eclipse_Projects\PelletBurnerV2\Release\arduino.ar \

ELF += \
PelletBurnerv2.elf \

HEX += \
PelletBurnerv2.hex \


# All Target
all: PelletBurnerv2

# Tool invocations
PelletBurnerv2: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-size" --format=avr --mcu=atmega328p "C:/Users/zick/Documents/Arduino/Arduino/automation/Eclipse/Eclipse_Projects/PelletBurnerV2/Release/PelletBurnerv2.elf"
	@echo 'Finished building target: $@'
	@echo ' '

C:\Users\zick\Documents\Arduino\Arduino\automation\Eclipse\Eclipse_Projects\PelletBurnerV2\Release\arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc-ar" rcs    C:/Users/zick/Documents/Arduino/Arduino/automation/Eclipse/Eclipse_Projects/PelletBurnerV2/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

PelletBurnerv2.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"C:\Users\zick\eclipse\cpp-oxygen\eclipse\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p -o "C:\Users\zick\Documents\Arduino\Arduino\automation\Eclipse\Eclipse_Projects\PelletBurnerV2/Release/PelletBurnerv2.elf"   $(LINK_OBJ)   C:/Users/zick/Documents/Arduino/Arduino/automation/Eclipse/Eclipse_Projects/PelletBurnerV2/Release/arduino.ar  "-LC:\Users\zick\Documents\Arduino\Arduino\automation\Eclipse\Eclipse_Projects\PelletBurnerV2/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

PelletBurnerv2.hex: $(ELF)
	@echo 'Create eeprom image'
	${A.RECIPE.OBJCOPY.EEP.PATTERN}
	${A.RECIPE.OBJCOPY.HEX.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ..\makefile.targets
