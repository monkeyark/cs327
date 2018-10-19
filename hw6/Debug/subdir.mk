################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dungeon.cpp \
../hw6.cpp \
../main.cpp \
../npcmove.cpp \
../queue.cpp 

OBJS += \
./dungeon.o \
./hw6.o \
./main.o \
./npcmove.o \
./queue.o 

CPP_DEPS += \
./dungeon.d \
./hw6.d \
./main.d \
./npcmove.d \
./queue.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


