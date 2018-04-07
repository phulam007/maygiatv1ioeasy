################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
may_giat_bak.lss \

FLASH_IMAGE += \
may_giat_bak.hex \

EEPROM_IMAGE += \
may_giat_bak.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: may_giat_bak.elf secondary-outputs

# Tool invocations
may_giat_bak.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,may_giat_bak.map -mmcu=atmega328p -o "may_giat_bak.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

may_giat_bak.lss: may_giat_bak.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S may_giat_bak.elf  >"may_giat_bak.lss"
	@echo 'Finished building: $@'
	@echo ' '

may_giat_bak.hex: may_giat_bak.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex may_giat_bak.elf  "may_giat_bak.hex"
	@echo 'Finished building: $@'
	@echo ' '

may_giat_bak.eep: may_giat_bak.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex may_giat_bak.elf  "may_giat_bak.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: may_giat_bak.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p may_giat_bak.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) may_giat_bak.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
