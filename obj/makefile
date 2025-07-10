################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include General_File/driver/subdir.mk
-include General_File/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
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
SECONDARY_FLASH += \
24G_rander_navi.hex \

SECONDARY_LIST += \
24G_rander_navi.lst \

SECONDARY_SIZE += \
24G_rander_navi.siz \


# 所有目标
all: 24G_rander_navi.elf secondary-outputs

# 工具调用
24G_rander_navi.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"24G_rander_navi.map" --specs=nano.specs -u _printf_float --specs=nosys.specs -o "24G_rander_navi.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
24G_rander_navi.hex: 24G_rander_navi.elf
	@	riscv-none-embed-objcopy -O ihex "24G_rander_navi.elf"  "24G_rander_navi.hex"
	@	@
24G_rander_navi.lst: 24G_rander_navi.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "24G_rander_navi.elf" > "24G_rander_navi.lst"
	@	@
24G_rander_navi.siz: 24G_rander_navi.elf
	@	riscv-none-embed-size --format=berkeley "24G_rander_navi.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) 24G_rander_navi.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
