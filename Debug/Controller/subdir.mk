################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Controller/Controller.cpp \
../Controller/Device.cpp \
../Controller/Minuit.cpp \
../Controller/OSC.cpp \
../Controller/PluginFactories.cpp \
../Controller/main.cpp 

OBJS += \
./Controller/Controller.o \
./Controller/Device.o \
./Controller/Minuit.o \
./Controller/OSC.o \
./Controller/PluginFactories.o \
./Controller/main.o 

CPP_DEPS += \
./Controller/Controller.d \
./Controller/Device.d \
./Controller/Minuit.d \
./Controller/OSC.d \
./Controller/PluginFactories.d \
./Controller/main.d 


# Each subdirectory must supply rules for building sources it contributes
Controller/%.o: ../Controller/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


