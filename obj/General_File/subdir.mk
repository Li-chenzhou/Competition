################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../General_File/init.c \
../General_File/interrupt.c \
../General_File/system.c 

OBJS += \
./General_File/init.o \
./General_File/interrupt.o \
./General_File/system.o 

C_DEPS += \
./General_File/init.d \
./General_File/interrupt.d \
./General_File/system.d 


# Each subdirectory must supply rules for building sources it contributes
General_File/%.o: ../General_File/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Debug" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Core" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\User" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Peripheral\inc" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\app" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

