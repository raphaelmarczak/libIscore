################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EnginesPrivate/EnginesPrivate.cpp 

OBJS += \
./EnginesPrivate/EnginesPrivate.o 

CPP_DEPS += \
./EnginesPrivate/EnginesPrivate.d 


# Each subdirectory must supply rules for building sources it contributes
EnginesPrivate/%.o: ../EnginesPrivate/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


