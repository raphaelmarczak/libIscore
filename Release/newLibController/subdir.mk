################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../newLibController/Controller.cpp \
../newLibController/Device.cpp \
../newLibController/PluginFactories.cpp 

OBJS += \
./newLibController/Controller.o \
./newLibController/Device.o \
./newLibController/PluginFactories.o 

CPP_DEPS += \
./newLibController/Controller.d \
./newLibController/Device.d \
./newLibController/PluginFactories.d 


# Each subdirectory must supply rules for building sources it contributes
newLibController/%.o: ../newLibController/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -mmacosx-version-min=10.4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


