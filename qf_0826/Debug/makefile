################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Test/XmTest/subdir.mk
-include src/Test/EdfaTest/subdir.mk
-include src/Test/DebugUart/subdir.mk
-include src/Bsp/WatchDog/subdir.mk
-include src/Bsp/UartNs550/subdir.mk
-include src/Bsp/UartLite/subdir.mk
-include src/Bsp/Timer/subdir.mk
-include src/Bsp/Spi/subdir.mk
-include src/Bsp/Platform/subdir.mk
-include src/Bsp/Intc/subdir.mk
-include src/Bsp/Gpio/subdir.mk
-include src/Bsp/Exception/subdir.mk
-include src/Bsp/Dma/subdir.mk
-include src/Bsp/Ddr/subdir.mk
-include src/Bsp/Bram/subdir.mk
-include src/Bsp/Axi/subdir.mk
-include src/Base/Comm/subdir.mk
-include src/App/Xm/subdir.mk
-include src/App/Wm/subdir.mk
-include src/App/Tm/subdir.mk
-include src/App/Tc/subdir.mk
-include src/App/Sc/subdir.mk
-include src/App/Sa/subdir.mk
-include src/App/Pp/subdir.mk
-include src/App/Pat/subdir.mk
-include src/App/IintChk/subdir.mk
-include src/App/Fsm/subdir.mk
-include src/App/Eml/subdir.mk
-include src/App/Edfa/subdir.mk
-include src/App/Cmd/subdir.mk
-include src/App/Cg/subdir.mk
-include src/App/Apd/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
qf_0826.elf.size \


# All Target
all: qf_0826.elf secondary-outputs

# Tool invocations
qf_0826.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../qf_0826_bsp/microblaze_0/lib -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -Wl,--gc-sections -o "qf_0826.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

qf_0826.elf.size: qf_0826.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size qf_0826.elf  |tee "qf_0826.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) qf_0826.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
