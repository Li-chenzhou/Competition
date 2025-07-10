################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../General_File/driver/Air780e.c \
../General_File/driver/OLED.c \
../General_File/driver/asr.c \
../General_File/driver/mpu.c \
../General_File/driver/rander.c 

OBJS += \
./General_File/driver/Air780e.o \
./General_File/driver/OLED.o \
./General_File/driver/asr.o \
./General_File/driver/mpu.o \
./General_File/driver/rander.o 

C_DEPS += \
./General_File/driver/Air780e.d \
./General_File/driver/OLED.d \
./General_File/driver/asr.d \
./General_File/driver/mpu.d \
./General_File/driver/rander.d 


# Each subdirectory must supply rules for building sources it contributes
General_File/driver/%.o: ../General_File/driver/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Debug" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Core" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\User" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\Peripheral\inc" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\app" -I"D:\Code\MRS\CH32V307VCT6 (1)\7-8error-mid\CH32V307VCT6\General_File\driver" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

