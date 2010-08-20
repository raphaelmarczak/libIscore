################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Controller/NetworkCommunication/osc/OscOutboundPacketStream.cpp \
../Controller/NetworkCommunication/osc/OscPrintReceivedElements.cpp \
../Controller/NetworkCommunication/osc/OscReceivedElements.cpp \
../Controller/NetworkCommunication/osc/OscTypes.cpp 

O_SRCS += \
../Controller/NetworkCommunication/osc/OscOutboundPacketStream.o \
../Controller/NetworkCommunication/osc/OscPrintReceivedElements.o \
../Controller/NetworkCommunication/osc/OscReceivedElements.o \
../Controller/NetworkCommunication/osc/OscTypes.o 

OBJS += \
./Controller/NetworkCommunication/osc/OscOutboundPacketStream.o \
./Controller/NetworkCommunication/osc/OscPrintReceivedElements.o \
./Controller/NetworkCommunication/osc/OscReceivedElements.o \
./Controller/NetworkCommunication/osc/OscTypes.o 

CPP_DEPS += \
./Controller/NetworkCommunication/osc/OscOutboundPacketStream.d \
./Controller/NetworkCommunication/osc/OscPrintReceivedElements.d \
./Controller/NetworkCommunication/osc/OscReceivedElements.d \
./Controller/NetworkCommunication/osc/OscTypes.d 


# Each subdirectory must supply rules for building sources it contributes
Controller/NetworkCommunication/osc/%.o: ../Controller/NetworkCommunication/osc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -I"/Users/Raphael/Documents/workspace/libIscore/Editor/CSP" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


