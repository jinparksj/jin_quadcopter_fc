################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
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
12_2_final_jin_quadcopter.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
12_2_final_jin_quadcopter.list \

OBJCOPY_BIN += \
12_2_final_jin_quadcopter.bin \


# All Target
all: 12_2_final_jin_quadcopter.elf secondary-outputs

# Tool invocations
12_2_final_jin_quadcopter.elf: $(OBJS) $(USER_OBJS) /home/jin/STM32CubeIDE/workspace_1.3.0/12_2_final_jin_quadcopter/STM32F405RGTX_FLASH.ld
	arm-none-eabi-gcc -o "12_2_final_jin_quadcopter.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jin/STM32CubeIDE/workspace_1.3.0/12_2_final_jin_quadcopter/STM32F405RGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="12_2_final_jin_quadcopter.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

12_2_final_jin_quadcopter.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "12_2_final_jin_quadcopter.list"
	@echo 'Finished building: $@'
	@echo ' '

12_2_final_jin_quadcopter.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "12_2_final_jin_quadcopter.bin"
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
