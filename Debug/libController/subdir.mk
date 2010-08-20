################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libController/Controller.cpp \
../libController/Device.cpp \
../libController/PluginFactories.cpp 

OBJS += \
./libController/Controller.o \
./libController/Device.o \
./libController/PluginFactories.o 

CPP_DEPS += \
./libController/Controller.d \
./libController/Device.d \
./libController/PluginFactories.d 


# Each subdirectory must supply rules for building sources it contributes
libController/%.o: ../libController/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


