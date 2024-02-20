#!/bin/bash

#####################################################
# System: Quantum Origin Onboard
# Component: QO-Onboard C-Library
# File: qoo_samples_install.sh
# Description: Script to install QO-Onboard C-Library
#####################################################

# Debugging
#set -x

# Halt on error
set -e


function fnInstall()
{
	mkdir -p ${INSTALL_TARGET}

	# Ensure that we have the tarball here with us
	for i in qo_onboard_samples__*.gz; do
	  if [ ! -f "$i" ]; then
	    echo "ERROR: Failed to locate qo_onboard_samples__*.gz published tarball. Exiting."
	    exit 1
	  fi
	done

	# Extract files from the tarball
	tar -zxvf qo_onboard_samples__*.gz --directory ${INSTALL_TARGET}

	if [ ! -d ${INSTALL_TARGET} ]
	then
		echo -e "ERROR: Unable to create install directory: ${INSTALL_TARGET}/ - Please check permissions"
		exit
	fi
}

###################################
# main
###################################
INSTALL_TARGET=${QOO_INSTALL_DIRECTORY:-${HOME}/quantum-origin-onboard}

echo -e "INFO: Installing Quantum Origin Samples to directory: ${INSTALL_TARGET}/"
fnInstall
echo -e "INFO: Quantum Origin Samples successfully installed to directory: ${INSTALL_TARGET}/"
echo -e "INFO: Next steps would typically be:"
echo -e "INFO:    cd ${INSTALL_TARGET}/"
echo -e "INFO:    qoo_samples_build.sh"
echo -e "INFO:    qoo_samples_run.sh"
