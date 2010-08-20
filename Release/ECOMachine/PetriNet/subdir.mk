################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ECOMachine/PetriNet/Arc.cpp \
../ECOMachine/PetriNet/ExtendedInt.cpp \
../ECOMachine/PetriNet/PetriNet.cpp \
../ECOMachine/PetriNet/PetriNetItem.cpp \
../ECOMachine/PetriNet/PetriNetNode.cpp \
../ECOMachine/PetriNet/Place.cpp \
../ECOMachine/PetriNet/PriorityTransitionAction.cpp \
../ECOMachine/PetriNet/ThreadSafeList.cpp \
../ECOMachine/PetriNet/Token.cpp \
../ECOMachine/PetriNet/Transition.cpp \
../ECOMachine/PetriNet/TransitionAction.cpp \
../ECOMachine/PetriNet/TransitionBitArray.cpp 

OBJS += \
./ECOMachine/PetriNet/Arc.o \
./ECOMachine/PetriNet/ExtendedInt.o \
./ECOMachine/PetriNet/PetriNet.o \
./ECOMachine/PetriNet/PetriNetItem.o \
./ECOMachine/PetriNet/PetriNetNode.o \
./ECOMachine/PetriNet/Place.o \
./ECOMachine/PetriNet/PriorityTransitionAction.o \
./ECOMachine/PetriNet/ThreadSafeList.o \
./ECOMachine/PetriNet/Token.o \
./ECOMachine/PetriNet/Transition.o \
./ECOMachine/PetriNet/TransitionAction.o \
./ECOMachine/PetriNet/TransitionBitArray.o 

CPP_DEPS += \
./ECOMachine/PetriNet/Arc.d \
./ECOMachine/PetriNet/ExtendedInt.d \
./ECOMachine/PetriNet/PetriNet.d \
./ECOMachine/PetriNet/PetriNetItem.d \
./ECOMachine/PetriNet/PetriNetNode.d \
./ECOMachine/PetriNet/Place.d \
./ECOMachine/PetriNet/PriorityTransitionAction.d \
./ECOMachine/PetriNet/ThreadSafeList.d \
./ECOMachine/PetriNet/Token.d \
./ECOMachine/PetriNet/Transition.d \
./ECOMachine/PetriNet/TransitionAction.d \
./ECOMachine/PetriNet/TransitionBitArray.d 


# Each subdirectory must supply rules for building sources it contributes
ECOMachine/PetriNet/%.o: ../ECOMachine/PetriNet/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -arch i386 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


