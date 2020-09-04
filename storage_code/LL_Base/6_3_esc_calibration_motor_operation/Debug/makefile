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
6_3_esc_calibration_motor_operation.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
6_3_esc_calibration_motor_operation.list \

OBJCOPY_BIN += \
6_3_esc_calibration_motor_operation.bin \


# All Target
all: 6_3_esc_calibration_motor_operation.elf secondary-outputs

# Tool invocations
6_3_esc_calibration_motor_operation.elf: $(OBJS) $(USER_OBJS) /home/jin/STM32CubeIDE/workspace_1.3.0/6_3_esc_calibration_motor_operation/STM32F405RGTX_FLASH.ld
	arm-none-eabi-gcc -o "6_3_esc_calibration_motor_operation.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jin/STM32CubeIDE/workspace_1.3.0/6_3_esc_calibration_motor_operation/STM32F405RGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="6_3_esc_calibration_motor_operation.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

6_3_esc_calibration_motor_operation.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "6_3_esc_calibration_motor_operation.list"
	@echo 'Finished building: $@'
	@echo ' '

6_3_esc_calibration_motor_operation.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "6_3_esc_calibration_motor_operation.bin"
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
