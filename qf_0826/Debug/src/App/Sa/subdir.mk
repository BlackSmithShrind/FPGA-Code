################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/App/Sa/Gp_SA.c 

OBJS += \
./src/App/Sa/Gp_SA.o 

C_DEPS += \
./src/App/Sa/Gp_SA.d 


# Each subdirectory must supply rules for building sources it contributes
src/App/Sa/%.o: ../src/App/Sa/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


