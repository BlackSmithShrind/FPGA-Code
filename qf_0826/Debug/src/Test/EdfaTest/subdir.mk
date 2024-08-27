################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Test/EdfaTest/Gp_EdfaTest.c 

OBJS += \
./src/Test/EdfaTest/Gp_EdfaTest.o 

C_DEPS += \
./src/Test/EdfaTest/Gp_EdfaTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/Test/EdfaTest/%.o: ../src/Test/EdfaTest/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


