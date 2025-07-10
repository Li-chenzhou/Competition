################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Debug" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Core" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\User" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Peripheral\inc" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\app" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

