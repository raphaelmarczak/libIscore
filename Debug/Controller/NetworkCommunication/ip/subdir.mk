################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Controller/NetworkCommunication/ip/IpEndpointName.cpp \
../Controller/NetworkCommunication/ip/NetworkingUtils.cpp \
../Controller/NetworkCommunication/ip/UdpSocket.cpp 

O_SRCS += \
../Controller/NetworkCommunication/ip/IpEndpointName.o \
../Controller/NetworkCommunication/ip/NetworkingUtils.o \
../Controller/NetworkCommunication/ip/UdpSocket.o 

OBJS += \
./Controller/NetworkCommunication/ip/IpEndpointName.o \
./Controller/NetworkCommunication/ip/NetworkingUtils.o \
./Controller/NetworkCommunication/ip/UdpSocket.o 

CPP_DEPS += \
./Controller/NetworkCommunication/ip/IpEndpointName.d \
./Controller/NetworkCommunication/ip/NetworkingUtils.d \
./Controller/NetworkCommunication/ip/UdpSocket.d 


# Each subdirectory must supply rules for building sources it contributes
Controller/NetworkCommunication/ip/%.o: ../Controller/NetworkCommunication/ip/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -I"/Users/Raphael/Documents/workspace/libIscore/Editor/CSP" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


