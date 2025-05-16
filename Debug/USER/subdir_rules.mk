################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
USER/%.o: ../USER/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"E:/ti/CCS_THEIA/ccs/tools/compiler/ti-cgt-armllvm_4.0.0.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"E:/ti/workspace/02_Simple_Circuit_CH_Tester" -I"E:/ti/workspace/02_Simple_Circuit_CH_Tester/HARDWARE" -I"E:/ti/workspace/02_Simple_Circuit_CH_Tester/USER" -I"E:/ti/workspace/02_Simple_Circuit_CH_Tester/SYSTEM" -I"E:/ti/workspace/02_Simple_Circuit_CH_Tester/Debug" -I"E:/ti/SDK/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"E:/ti/SDK/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"USER/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


