################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ARDrawingContext.cpp \
../ARPipeline.cpp \
../CameraCalibration.cpp \
../GeometryTypes.cpp \
../Pattern.cpp \
../PatternDetector.cpp \
../main.cpp 

OBJS += \
./ARDrawingContext.o \
./ARPipeline.o \
./CameraCalibration.o \
./GeometryTypes.o \
./Pattern.o \
./PatternDetector.o \
./main.o 

CPP_DEPS += \
./ARDrawingContext.d \
./ARPipeline.d \
./CameraCalibration.d \
./GeometryTypes.d \
./Pattern.d \
./PatternDetector.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


