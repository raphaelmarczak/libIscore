################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Editor/CSP/AllenRelation.cpp \
../Editor/CSP/AntPostRelation.cpp \
../Editor/CSP/BinaryRelation.cpp \
../Editor/CSP/BinaryTemporalRelation.cpp \
../Editor/CSP/BoundingRelation.cpp \
../Editor/CSP/CSP.cpp \
../Editor/CSP/CSPConstrainedVariable.cpp \
../Editor/CSP/CSPLinearConstraint.cpp \
../Editor/CSP/ConstrainedBox.cpp \
../Editor/CSP/ConstrainedTemporalEntity.cpp \
../Editor/CSP/ControlPoint.cpp \
../Editor/CSP/IntervalRelation.cpp \
../Editor/CSP/TriggerPoint.cpp \
../Editor/CSP/XMLConversion.cpp 

OBJS += \
./Editor/CSP/AllenRelation.o \
./Editor/CSP/AntPostRelation.o \
./Editor/CSP/BinaryRelation.o \
./Editor/CSP/BinaryTemporalRelation.o \
./Editor/CSP/BoundingRelation.o \
./Editor/CSP/CSP.o \
./Editor/CSP/CSPConstrainedVariable.o \
./Editor/CSP/CSPLinearConstraint.o \
./Editor/CSP/ConstrainedBox.o \
./Editor/CSP/ConstrainedTemporalEntity.o \
./Editor/CSP/ControlPoint.o \
./Editor/CSP/IntervalRelation.o \
./Editor/CSP/TriggerPoint.o \
./Editor/CSP/XMLConversion.o 

CPP_DEPS += \
./Editor/CSP/AllenRelation.d \
./Editor/CSP/AntPostRelation.d \
./Editor/CSP/BinaryRelation.d \
./Editor/CSP/BinaryTemporalRelation.d \
./Editor/CSP/BoundingRelation.d \
./Editor/CSP/CSP.d \
./Editor/CSP/CSPConstrainedVariable.d \
./Editor/CSP/CSPLinearConstraint.d \
./Editor/CSP/ConstrainedBox.d \
./Editor/CSP/ConstrainedTemporalEntity.d \
./Editor/CSP/ControlPoint.d \
./Editor/CSP/IntervalRelation.d \
./Editor/CSP/TriggerPoint.d \
./Editor/CSP/XMLConversion.d 


# Each subdirectory must supply rules for building sources it contributes
Editor/CSP/%.o: ../Editor/CSP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++-4.2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


