################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/App/Wm/Gp_WM.c 

OBJS += \
./src/App/Wm/Gp_WM.o 

C_DEPS += \
./src/App/Wm/Gp_WM.d 


# Each subdirectory must supply rules for building sources it contributes
src/App/Wm/%.o: ../src/App/Wm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


