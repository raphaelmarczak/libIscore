################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Editor/gecode-solver/customSpace.cpp \
../Editor/gecode-solver/integerVariable.cpp \
../Editor/gecode-solver/linearConstraint.cpp \
../Editor/gecode-solver/searchEngine.cpp \
../Editor/gecode-solver/solver.cpp \
../Editor/gecode-solver/solver_wrap.cpp 

OBJS += \
./Editor/gecode-solver/customSpace.o \
./Editor/gecode-solver/integerVariable.o \
./Editor/gecode-solver/linearConstraint.o \
./Editor/gecode-solver/searchEngine.o \
./Editor/gecode-solver/solver.o \
./Editor/gecode-solver/solver_wrap.o 

CPP_DEPS += \
./Editor/gecode-solver/customSpace.d \
./Editor/gecode-solver/integerVariable.d \
./Editor/gecode-solver/linearConstraint.d \
./Editor/gecode-solver/searchEngine.d \
./Editor/gecode-solver/solver.d \
./Editor/gecode-solver/solver_wrap.d 


# Each subdirectory must supply rules for building sources it contributes
Editor/gecode-solver/%.o: ../Editor/gecode-solver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -arch i386 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


