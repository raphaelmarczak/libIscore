################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ECOMachine/Process/MultiTypeCurve.cpp \
../ECOMachine/Process/NetworkMessageCurves.cpp \
../ECOMachine/Process/Process.cpp \
../ECOMachine/Process/SendNetworkMessageProcess.cpp \
../ECOMachine/Process/StringParser.cpp \
../ECOMachine/Process/StringTypeFunctions.cpp 

OBJS += \
./ECOMachine/Process/MultiTypeCurve.o \
./ECOMachine/Process/NetworkMessageCurves.o \
./ECOMachine/Process/Process.o \
./ECOMachine/Process/SendNetworkMessageProcess.o \
./ECOMachine/Process/StringParser.o \
./ECOMachine/Process/StringTypeFunctions.o 

CPP_DEPS += \
./ECOMachine/Process/MultiTypeCurve.d \
./ECOMachine/Process/NetworkMessageCurves.d \
./ECOMachine/Process/Process.d \
./ECOMachine/Process/SendNetworkMessageProcess.d \
./ECOMachine/Process/StringParser.d \
./ECOMachine/Process/StringTypeFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
ECOMachine/Process/%.o: ../ECOMachine/Process/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -mmacosx-version-min=10.4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


