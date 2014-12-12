################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ASEC-Shooter.cpp \
../src/Background.cpp \
../src/Game.cpp 

OBJS += \
./src/ASEC-Shooter.o \
./src/Background.o \
./src/Game.o 

CPP_DEPS += \
./src/ASEC-Shooter.d \
./src/Background.d \
./src/Game.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


