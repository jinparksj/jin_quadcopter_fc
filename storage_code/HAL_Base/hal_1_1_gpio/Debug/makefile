################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_HOST/Target/subdir.mk
-include USB_HOST/App/subdir.mk
-include Middlewares/ST/STM32_USB_Host_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/subdir.mk
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
hal_1_1_gpio.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
hal_1_1_gpio.list \

OBJCOPY_BIN += \
hal_1_1_gpio.bin \


# All Target
all: hal_1_1_gpio.elf secondary-outputs

# Tool invocations
hal_1_1_gpio.elf: $(OBJS) $(USER_OBJS) /home/jin/STM32CubeIDE/workspace_1.3.0/hal_1_1_gpio/STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "hal_1_1_gpio.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/jin/STM32CubeIDE/workspace_1.3.0/hal_1_1_gpio/STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="hal_1_1_gpio.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

hal_1_1_gpio.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "hal_1_1_gpio.list"
	@echo 'Finished building: $@'
	@echo ' '

hal_1_1_gpio.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "hal_1_1_gpio.bin"
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
