################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../StoryLine/StoryLine.cpp 

OBJS += \
./StoryLine/StoryLine.o 

CPP_DEPS += \
./StoryLine/StoryLine.d 


# Each subdirectory must supply rules for building sources it contributes
StoryLine/%.o: ../StoryLine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -mmacosx-version-min=10.4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


