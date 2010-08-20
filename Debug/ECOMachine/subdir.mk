################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ECOMachine/ChangeTempo.cpp \
../ECOMachine/ControlPointInformations.cpp \
../ECOMachine/ECOMachine.cpp \
../ECOMachine/TriggerPointInformations.cpp 

OBJS += \
./ECOMachine/ChangeTempo.o \
./ECOMachine/ControlPointInformations.o \
./ECOMachine/ECOMachine.o \
./ECOMachine/TriggerPointInformations.o 

CPP_DEPS += \
./ECOMachine/ChangeTempo.d \
./ECOMachine/ControlPointInformations.d \
./ECOMachine/ECOMachine.d \
./ECOMachine/TriggerPointInformations.d 


# Each subdirectory must supply rules for building sources it contributes
ECOMachine/%.o: ../ECOMachine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


