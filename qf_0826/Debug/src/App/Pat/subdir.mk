################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/App/Pat/Gp_CalcTrack6Numbers.c \
../src/App/Pat/Gp_Get_E.c \
../src/App/Pat/Gp_Get_M.c \
../src/App/Pat/Gp_Get_f.c \
../src/App/Pat/Gp_MatrixOperation.c 

OBJS += \
./src/App/Pat/Gp_CalcTrack6Numbers.o \
./src/App/Pat/Gp_Get_E.o \
./src/App/Pat/Gp_Get_M.o \
./src/App/Pat/Gp_Get_f.o \
./src/App/Pat/Gp_MatrixOperation.o 

C_DEPS += \
./src/App/Pat/Gp_CalcTrack6Numbers.d \
./src/App/Pat/Gp_Get_E.d \
./src/App/Pat/Gp_Get_M.d \
./src/App/Pat/Gp_Get_f.d \
./src/App/Pat/Gp_MatrixOperation.d 


# Each subdirectory must supply rules for building sources it contributes
src/App/Pat/%.o: ../src/App/Pat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../qf_0826_bsp/microblaze_0/include -mlittle-endian -mcpu=v11.0 -mno-xl-soft-mul -mhard-float -mxl-float-convert -mxl-float-sqrt -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


