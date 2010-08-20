################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Controller/Minuit/MinuitCommunicationMethods.cpp \
../Controller/Minuit/MinuitGetAnswer.cpp \
../Controller/Minuit/MinuitNamespaceAnswer.cpp \
../Controller/Minuit/OSCFunctions.cpp \
../Controller/Minuit/OSCParser.cpp \
../Controller/Minuit/ReceiveOSCThread.cpp 

O_SRCS += \
../Controller/Minuit/MinuitCommunicationMethods.o \
../Controller/Minuit/MinuitGetAnswer.o \
../Controller/Minuit/MinuitNamespaceAnswer.o \
../Controller/Minuit/OSCFunctions.o \
../Controller/Minuit/OSCParser.o \
../Controller/Minuit/ReceiveOSCThread.o 

OBJS += \
./Controller/Minuit/MinuitCommunicationMethods.o \
./Controller/Minuit/MinuitGetAnswer.o \
./Controller/Minuit/MinuitNamespaceAnswer.o \
./Controller/Minuit/OSCFunctions.o \
./Controller/Minuit/OSCParser.o \
./Controller/Minuit/ReceiveOSCThread.o 

CPP_DEPS += \
./Controller/Minuit/MinuitCommunicationMethods.d \
./Controller/Minuit/MinuitGetAnswer.d \
./Controller/Minuit/MinuitNamespaceAnswer.d \
./Controller/Minuit/OSCFunctions.d \
./Controller/Minuit/OSCParser.d \
./Controller/Minuit/ReceiveOSCThread.d 


# Each subdirectory must supply rules for building sources it contributes
Controller/Minuit/%.o: ../Controller/Minuit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -I"/Users/Raphael/Documents/workspace/libIscore/Editor/CSP" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


