################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/App/Pp/Gp_PowerPanel.c 

OBJS += \
./src/App/Pp/Gp_PowerPanel.o 

C_DEPS += \
./src/App/Pp/Gp_PowerPanel.d 


# Each subdirectory must supply rules for building sources it contributes
src/App/Pp/%.o: ../src/App/Pp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


