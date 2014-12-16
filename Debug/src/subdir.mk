################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ASEC-Shooter.cpp \
../src/Background.cpp \
../src/Bullet.cpp \
../src/EnemiesManager.cpp \
../src/Enemy.cpp \
../src/Game.cpp \
../src/Player.cpp \
../src/Tools.cpp 

OBJS += \
./src/ASEC-Shooter.o \
./src/Background.o \
./src/Bullet.o \
./src/EnemiesManager.o \
./src/Enemy.o \
./src/Game.o \
./src/Player.o \
./src/Tools.o 

CPP_DEPS += \
./src/ASEC-Shooter.d \
./src/Background.d \
./src/Bullet.d \
./src/EnemiesManager.d \
./src/Enemy.d \
./src/Game.d \
./src/Player.d \
./src/Tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


