################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LcdDriver/%.obj: ../LcdDriver/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/arthur/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/home/arthur/ti/ccs1200/ccs/ccs_base/msp430/include" --include_path="/home/arthur/ece2049/workspace/Lab4/Debug" --include_path="/home/arthur/ece2049/workspace/Lab4/fonts" --include_path="/home/arthur/ece2049/workspace/Lab4/grlib" --include_path="/home/arthur/ece2049/workspace/Lab4/LcdDriver" --include_path="/home/arthur/ece2049/workspace/Lab4/lib" --include_path="/home/arthur/ece2049/workspace/Lab4" --include_path="/home/arthur/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="LcdDriver/$(basename $(<F)).d_raw" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


