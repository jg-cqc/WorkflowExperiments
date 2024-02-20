#!/bin/bash

#####################################################
# System: Quantum Origin Onboard
# Component: QO-Onboard C-Library
# File: qoo_samples_build.sh
# Description: Script to build QO Onboard sample code
#####################################################

# Debugging
#set -x

# Halt on error
set -e


function fnBuildSamplesDynamicallyLinked()
{
	echo "--- Building dynamically linked samples"
	pushd ${INSTALL_TARGET}
	gcc -Wall -I${INSTALL_TARGET}/include -o ./qo_onboard_sample_code_A_using_config_file_dll.out ./qo_onboard_sample_code_A_using_config_file.c -lm -L${INSTALL_TARGET}/lib -lqo_onboard_library
	gcc -Wall -I${INSTALL_TARGET}/include -o ./qo_onboard_sample_code_B_using_setopt_api_dll.out ./qo_onboard_sample_code_B_using_setopt_api.c -lm -L${INSTALL_TARGET}/lib -lqo_onboard_library
	gcc -Wall -I${INSTALL_TARGET}/include -o ./qo_onboard_sample_code_C_minimal_get32bytes_dll.out ./qo_onboard_sample_code_C_minimal_get32bytes.c -lm -L${INSTALL_TARGET}/lib -lqo_onboard_library
	gcc -Wall -I${INSTALL_TARGET}/include -o ./qo_onboard_sample_code_D_minimal_coinflip_dll.out ./qo_onboard_sample_code_D_minimal_coinflip.c -lm -L${INSTALL_TARGET}/lib -lqo_onboard_library
	popd
}

###################################
# main
###################################
INSTALL_TARGET=${QOO_INSTALL_DIRECTORY:-${HOME}/quantum-origin-onboard}

fnBuildSamplesDynamicallyLinked
