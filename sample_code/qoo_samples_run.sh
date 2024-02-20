#!/bin/bash

#####################################################
# System: Quantum Origin Onboard
# Component: QO-Onboard C-Library
# File: qoo_samples_run.sh
# Description: Script to run QO Onboard sample code
#####################################################

# Debugging
set -x

# Halt on error
set -e


function fnRunSamplesDynamicallyLinked()
{
	echo "--- Building dynamically linked samples"

	# LD_PRELOAD of libasan.so is required for the debug builds (if not statically linked).
	PRELOAD_LIBASAN="/usr/lib/x86_64-linux-gnu/libasan.so.6.0.0"

	QOO_SAMPLE_CONFIGFILE=${INSTALL_TARGET}/qo_onboard_sample_config.yml
	QOO_SAMPLE_CONFIGNODEPATH=generator

	echo "** **********************************************************************"

	echo "** Running: LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_A_using_config_file_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH} --verbose"
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib" ${INSTALL_TARGET}/qo_onboard_sample_code_A_using_config_file_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH} --verbose
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib" ${INSTALL_TARGET}/qo_onboard_sample_code_A_using_config_file_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH} --verbose
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib" ${INSTALL_TARGET}/qo_onboard_sample_code_A_using_config_file_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH} --verbose 64
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib" ${INSTALL_TARGET}/qo_onboard_sample_code_A_using_config_file_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH} 400 | ent
	echo "** **********************************************************************"

	echo "** Running: LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_B_using_setopt_api_dll.out 16 --verbose"
	set +e # Permit error code 13811 "Seed content is same byte." caused by the dummy, all-zeroes seed in qo_onboard_sample_code_seed.h
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_B_using_setopt_api_dll.out 16 --verbose
	set -e # Restore previous "Halt on error" state
	echo "** **********************************************************************"

	echo "** Running: LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_C_minimal_get32bytes_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH}"
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_C_minimal_get32bytes_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH}
	echo "** **********************************************************************"

	echo "** Running: LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_D_minimal_coinflip_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH}"
	LD_PRELOAD=${PRELOAD_LIBASAN} LD_LIBRARY_PATH="${INSTALL_TARGET}/lib"  ${INSTALL_TARGET}/qo_onboard_sample_code_D_minimal_coinflip_dll.out ${QOO_SAMPLE_CONFIGFILE} ${QOO_SAMPLE_CONFIGNODEPATH}
	echo "** **********************************************************************"

	#echo "--- Run Dynamically linked samples Done"
}

###################################
# main
###################################
INSTALL_TARGET=${QOO_INSTALL_DIRECTORY:-${HOME}/quantum-origin-onboard}
fnRunSamplesDynamicallyLinked
