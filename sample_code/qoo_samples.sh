#!/bin/bash

#####################################################
# System: Quantum Origin Onboard
# Component: QO-Onboard C-Library
# File: qoo_samples.sh
# Description: Test script for QO-Onboard C-Library
#####################################################

# Debugging
#set -x

# Halt on error
set -e


function fnInstall()
{
	./qoo_samples_install.sh
}


function fnBuild()
{
	./qoo_samples_build.sh
}

function fnBuildAll()
{
	./qoo_samples_install.sh
	./qoo_samples_build.sh
	./qoo_samples_run.sh
}

function fnRun()
{
	./qoo_samples_run.sh
}

function fnBuildAndRun()
{
	QOO_INSTALL_DIRECTORY=. ./qoo_samples_build.sh
	QOO_INSTALL_DIRECTORY=. ./qoo_samples_run.sh
}

function fnClean()
{
	rm -f *.o *.out
}

function fnCleanAll()
{
	fnClean
	rm -rf ${INSTALL_TARGET}
}

###################################
# help
###################################
function fnHelp()
{
	echo "**********************************************"
	echo "*** Project: Quantum Origin Onboard Samples"
	echo "*** File   : sample_code/qoo_samples.sh"
	echo "*** Usage  : sample_code/qoo_samples.sh [arg]"
	echo "*** arg    : help"
	echo "***          install"
	echo "***          build"
	echo "***          buildall"
	echo "***          run"
	echo "***          clean"
	echo "***          cleanall"
	echo "**********************************************"
}

###################################
# main
###################################
INSTALL_TARGET=${QOO_INSTALL_DIRECTORY:-${HOME}/quantum-origin-onboard}

if   [[ "$1" = "help" ]]          ; then fnHelp
elif [[ "$1" = "install" ]]       ; then fnInstall
elif [[ "$1" = "build" ]]         ; then fnBuild
elif [[ "$1" = "buildall" ]]      ; then fnBuildAll
elif [[ "$1" = "run" ]]           ; then fnRun
elif [[ "$1" = "build_and_run" ]] ; then fnBuildAndRun
elif [[ "$1" = "clean" ]]         ; then fnClean
elif [[ "$1" = "cleanall" ]]      ; then fnCleanAll
else
	fnCleanAll
	fnInstall
	fnBuildAll
fi
