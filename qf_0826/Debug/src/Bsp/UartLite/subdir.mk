################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Bsp/UartLite/Gp_UartLite.c 

OBJS += \
./src/Bsp/UartLite/Gp_UartLite.o 

C_DEPS += \
./src/Bsp/UartLite/Gp_UartLite.d 


# Each subdirectory must supply rules for building sources it contributes
src/Bsp/UartLite/%.o: ../src/Bsp/UartLite/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


