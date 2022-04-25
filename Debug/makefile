################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EDesign\ Project.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EDesign\ Project.list \

OBJCOPY_BIN += \
EDesign\ Project.bin \


# All Target
all: EDesign\ Project.elf secondary-outputs

# Tool invocations
EDesign\ Project.elf: $(OBJS) $(USER_OBJS) C:\University\EDesign314\EDesign\ Project\EDesign\ Project\STM32F303RETX_FLASH.ld
	arm-none-eabi-gcc -o "EDesign Project.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\University\EDesign314\EDesign Project\EDesign Project\STM32F303RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EDesign Project.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EDesign\ Project.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EDesign Project.list"
	@echo 'Finished building: $@'
	@echo ' '

EDesign\ Project.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "EDesign Project.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
