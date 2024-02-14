#!/bin/bash

#####################################################
# System: Quantum Origin
# Component: QuantumOrigin.Library.Onboard - A wrapper for the QO Onboard extractor
# File: build_and_install.sh
# Description: Parameter driven script to perform common/useful dev tasks.
#####################################################

# Debugging
#set -x

# Halt on error
set -e

# DECLARE SOME CONSTANTS
# When $TERM is empty (non-interactive shell e.g. github actions/CI), then expand tput with ' -T xterm-256color'
[[ ${TERM} == "" ]] && TPUTTERM=' -T xterm-256color' || TPUTTERM=''
BOLD1=$(tput${TPUTTERM} bold)
BOLD3=$(tput${TPUTTERM} bold)

BOLD_GREY="$(   tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 0)" # Grey
BOLD_RED="$(    tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 1)" # Red
BOLD_GREEN="$(  tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 2)" # Green  i.e. # Set Forground to Bold + Green(ANSI 2). Equivalent to "$(tput bold green)"
BOLD_BROWN="$(  tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 3)" # Brown (not yellow - and I have no reason why not)
BOLD_BLUE="$(   tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 4)" # Blue
BOLD_MAGENTA="$(tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 5)" # Magenta
BOLD_CYAN="$(   tput${TPUTTERM} bold)$(tput${TPUTTERM} setaf 6)" # Cyan   i.e. Set Forground to Bold + Cyan(ANSI 6). Equivalent to "$(tput bold cyan)"
CLEAR=$(tput${TPUTTERM} sgr0)

BROWN_FG="\e[33m"
BROWN_BG="\e[43m"
BLUE_BG="\e[44m"


# Bright Green:
# Escape can be \e or \u001b
#                   Reset       : \e[0m
#                   Black       : \e[30m
#                   Red         : \e[31m
#                   Green       : \e[32m
#                   Yellow      : \e[33m
#                   Blue        : \e[34m
#                   Magenta     : \e[35m
#                   Cyan        : \e[36m
#                   White       : \e[37m
#            Bright Black       : \e[30;1m
#            Bright Red         : \e[31;1m
#            Bright Green       : \e[32;1m    [... or \e[96 ? (64+32) ]
#            Bright Yellow      : \e[33;1m
#            Bright Blue        : \e[34;1m
#            Bright Magenta     : \e[35;1m
#            Bright Cyan        : \e[36;1m
#            Bright White       : \e[37;1m
# Background        Black       : \e[40m
# Background        Red         : \e[41m
# Background        Green       : \e[42m
# Background        Yellow      : \e[43m
# Background        Blue        : \e[44m
# Background        Magenta     : \e[45m
# Background        Cyan        : \e[46m
# Background        White       : \e[47m
# Background Bright Black       : \e[40;1m
# Background Bright Red         : \e[41;1m
# Background Bright Green       : \e[42;1m
# Background Bright Yellow      : \e[43;1m
# Background Bright Blue        : \e[44;1m
# Background Bright Magenta     : \e[45;1m
# Background Bright Cyan        : \e[46;1m
# Background Bright White       : \e[47;1m




function fnInstallDependencies()
{
	echo "TODO: fnInstallDependencies"
}

function fnGetCrossCompileDependencies()
{
	sudo apt update
	sudo apt install -y "gcc-mingw-w64" "g++-mingw-w64"
}

function fnQuickBuild()
{
	echo "TODO: fnQuickBuild"
}

function fnBuild()
{
	echo "TODO: fnBuild"
	#fnRunEndtoEndTests
}

function fnRebuild()
{
	echo "TODO: fnRebuild"
	rm -rf bin lib build
}

function fnRunEndtoEndTests()
{
	./build/bin/EndtoEndTests
}

function fnTests()
{
	erc=0 # Initialise error code to "success"

	echo "${BOLD1}*** Running Tests${CLEAR}"
	pushd tests

	echo -e "${BLUE_BG}--- UnitTests${CLEAR}"
	../build/bin/UnitTests || erc=1

	echo -e "${BLUE_BG}--- EndtoEndTests${CLEAR}"
	../build/bin/EndtoEndTests || erc=1

	echo -e "${BLUE_BG}--- IntegrationTests${CLEAR}"
	../build/bin/IntegrationTests || erc=1

	popd
	echo "${BOLD1}*** Tests Done${CLEAR}"

	fnSimpleCLITests || erc=1

	return $erc
}

function fnSimpleCLITests()
{
	erc=0 # Initialise error code to "success"

	echo "${BOLD1}*** Cleaning SimpleCLI_c Tests${CLEAR}"
	rm -f ./build/bin/qo_onboard_simple_cli_c
	rm -f ./build/bin/qo_onboard_simple_cli_using_setoptions_c

	echo "${BOLD1}*** Building SimpleCLI_c Tests${CLEAR}"
	fnQuickBuild

	echo "${BOLD1}*** Running SimpleCLI_c Tests${CLEAR}"
	pushd tests

	echo -e "${BLUE_BG}--- qo_onboard_simple_cli_c (hexdump)${CLEAR}"
	../build/bin/qo_onboard_simple_cli_c ../src/simple_cli_c/config_for_SimpleCLI_tests.yml 400 --verbose || erc=1

	echo -e "${BLUE_BG}--- qo_onboard_simple_cli_c (entropy analysis)${CLEAR}"
	../build/bin/qo_onboard_simple_cli_c ../src/simple_cli_c/config_for_SimpleCLI_tests.yml 400 | ent || erc=1

	echo -e "${BLUE_BG}--- qo_onboard_simple_cli_using_setoptions_c (hexdump)${CLEAR}"
	../build/bin/qo_onboard_simple_cli_using_setoptions_c 400 --verbose || erc=1

	echo -e "${BLUE_BG}--- qo_onboard_simple_cli_using_setoptions_c (entropy analysis)${CLEAR}"
	../build/bin/qo_onboard_simple_cli_using_setoptions_c 400 | ent || erc=1

	popd
	echo "${BOLD1}*** Tests Done${CLEAR}"

	return $erc
}

function fnClean()
{
	rm -rf ./build
	rm -rf ./documentation/build
	rm -rf ./documentation/html
	rm -rf ./documentation/latex
	rm -rf ./x64
	rm -rf ./out

	# ...And some others
	rm -rf ./Testing/Temporary/
	rm -rf ./Testing/
	rm -rf ./build_make/
	rm -rf ./package/
}

function fnCleanAll()
{
	fnClean
}

function fnPublish()
{
	mkdir -p publish
	cp build/libqo_onboard.so publish/
	cp -R include/ publish/
	cp -R sample_code/ publish/
	tar zcvf qo_onboard.tar.gz publish
	rm -rf publish
}

function fnPublishWindows()
{
	mkdir -p publish
	cp build/bin/libqo_onboard.dll publish/
	cp -R include/ publish/
	cp -R sample_code/ publish/
	tar zcvf qo_onboard.tar.gz publish
	rm -rf publish
}

#########################################
## Sample code section - BEGIN
#########################################

function fnGetDistro()
{
	################################################
	# Ubuntu
	#   $ lsb_release -si
	#   Ubuntu
	#   $ lsb_release -sr
	#   20.04

	# Centos
	#   $ cat /etc/centos-release
	#   CentOS Linux release 7.9.2009 (Core)
	#   $ cat /etc/centos-release | cut -d' ' -f1
	#   CentOS
	#   $ cat /etc/centos-release | awk -F 'release ' '{print $2}' | cut -d' ' -f1
	#   7.9.2009
	################################################

	# Determine OS platform
	# https://askubuntu.com/questions/459402/how-to-know-if-the-running-platform-is-ubuntu-or-centos-with-help-of-a-bash-scri
	UNAME=$(uname | tr "[:upper:]" "[:lower:]")
	# If Linux, try to determine specific distribution
	if [ "$UNAME" == "linux" ]
	then
		# If available, use LSB to identify distribution
		if [ -x /usr/bin/apt ] && [ ! -x /usr/bin/lsb_release ]
		then
			sudo apt install -y lsb-release
		fi
		#if [ -f /etc/lsb-release ] || [ -d /etc/lsb-release.d ]
		if [ -x /usr/bin/lsb_release ]
		then
			export OS_DISTRO_NAME=$(lsb_release -si)
			export OS_DISTRO_RELEASE=$(lsb_release -sr)
		elif [ -f /etc/redhat-release ]
		then
			export OS_DISTRO_NAME=$(cat /etc/redhat-release | cut -d' ' -f1)
			export OS_DISTRO_RELEASE=$(cat /etc/redhat-release | awk -F 'release ' '{print $2}' | cut -d' ' -f1)
		# Otherwise, use release info file
		else
			export OS_DISTRO_NAME=$(ls -d /etc/[A-Za-z]*[_-][rv]e[lr]* | grep -v "lsb" | cut -d'/' -f3 | cut -d'-' -f1 | cut -d'_' -f1)
		fi
	fi
	# For everything else (or if above failed), just use generic identifier
	[ "$OS_DISTRO_NAME" == "" ] && export OS_DISTRO_NAME=$UNAME
	[ "$OS_DISTRO_RELEASE" == "" ] && export OS_DISTRO_RELEASE=Unknown
	unset UNAME
}


function fnInstall_QuantumOriginOnboardFromGithub()
{
	mkdir -p ${INSTALL_TARGET}
	pushd ${INSTALL_TARGET}

	# Retrieve Quantum Origin Onboard release tarball
	wget https://github.com/CQCL-DEV/QuantumOrigin.Library.Onboard/releases/download/v2.0.2/qo_onboard_ubuntu-20.04_x64_Release-v2.0.2.tgz

	# Ensure that we have the tarball here with us
	for i in qo_onboard_*.tgz; do
		if [ ! -f "$i" ]; then
			echo "ERROR: Failed to retrieve qo_onboard_*.gz published tarball. Exiting."
			exit 1
		fi
	done

	# Extract files from the tarball
	tar -zxvf qo_onboard_samples__*.gz --directory ${INSTALL_TARGET}
	popd
}

# -------------------------------------------------------------------------
# Usage:  fnSampleCodeCreateTarball <PROJECT_ROOTDIR> <PACKAGE_DIR> <PACKAGE_LIBRARY_BINARIES> <PACKAGE_SAMPLE_BINARIES> <PACKAGE_HTML_DOCUMENTATION>
#    e.g. fnSampleCodeCreateTarball . ./build/package_sample_code false true
# -------------------------------------------------------------------------
function fnSampleCodeCreateTarball()
{
	echo -e "--- --------------------------------------------------------------------------------------------------------------------"
	echo -e "--- fnSampleCodeCreateTarball - BEGIN"

	PROJECT_ROOTDIR="${1}"
	PACKAGE_DIR="${2}"
	PACKAGE_LIBRARY_BINARIES="${3}"
	PACKAGE_SAMPLE_BINARIES="${4}"
	PACKAGE_HTML_DOCUMENTATION="${5}"
	echo "    PROJECT_ROOTDIR            = ${PROJECT_ROOTDIR}"
	echo "    PACKAGE_DIR                = ${PACKAGE_DIR}"
	echo "    PACKAGE_LIBRARY_BINARIES   = ${PACKAGE_LIBRARY_BINARIES}"
	echo "    PACKAGE_SAMPLE_BINARIES    = ${PACKAGE_SAMPLE_BINARIES}"
	echo "    PACKAGE_HTML_DOCUMENTATION = ${PACKAGE_HTML_DOCUMENTATION}"

	rm -rf "${PACKAGE_DIR}/"
	mkdir -p "${PACKAGE_DIR}/"

	if "${PACKAGE_LIBRARY_BINARIES}"
	then
		echo -e "--- Packaging library binaries..."
		mkdir -p "${PACKAGE_DIR}/lib/"
		cp ${PROJECT_ROOTDIR}/build/src/qo_onboard_c/libqo_onboard_c.a                   ${PACKAGE_DIR}/lib
		cp ${PROJECT_ROOTDIR}/build/src/qo_onboard_c/libqo_onboard_library.so            ${PACKAGE_DIR}/lib
		cp ${PROJECT_ROOTDIR}/build/src/qo_onboard_c/libqo_onboard_library.so.2.0.2      ${PACKAGE_DIR}/lib

		mkdir -p "${PACKAGE_DIR}/include/qo_onboard/"
		cp -r  ${PROJECT_ROOTDIR}/build_conan_package/include/qo_onboard/qo_onboard_c.h ${PACKAGE_DIR}/include/qo_onboard/
	fi

	if "${PACKAGE_HTML_DOCUMENTATION}"
	then
		echo -e "--- Packaging documentation..."
		cp -r  ${PROJECT_ROOTDIR}/documentation/public/latest/      ${PACKAGE_DIR}
		mv ${PACKAGE_DIR}/latest ${PACKAGE_DIR}/documentation
	fi

	cp ${PROJECT_ROOTDIR}/sample_code/Config.md                                      ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/README.md                                      ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_code_A_using_config_file.c   ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_code_B_using_setopt_api.c    ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_code_C_minimal_get32bytes.c  ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_code_D_minimal_coinflip.c    ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_code_seed.h                  ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qo_onboard_sample_config.yml                   ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/Makefile                                       ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qoo_samples.sh                                 ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qoo_samples_build.sh                           ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qoo_samples_install.sh                         ${PACKAGE_DIR}/
	cp ${PROJECT_ROOTDIR}/sample_code/qoo_samples_run.sh                             ${PACKAGE_DIR}/

	if "${PACKAGE_SAMPLE_BINARIES}"
	then
		echo -e "--- Packaging sample binaries..."
		mkdir -p ${PACKAGE_DIR}/bin
		cp ${PROJECT_ROOTDIR}/build/bin/qo_onboard_sample_code_A_using_config_file    ${PACKAGE_DIR}/bin/
		cp ${PROJECT_ROOTDIR}/build/bin/qo_onboard_sample_code_B_using_setopt_api     ${PACKAGE_DIR}/bin/
		cp ${PROJECT_ROOTDIR}/build/bin/qo_onboard_sample_code_C_minimal_get32bytes   ${PACKAGE_DIR}/bin/
		cp ${PROJECT_ROOTDIR}/build/bin/qo_onboard_sample_code_D_minimal_coinflip     ${PACKAGE_DIR}/bin/
	fi

	now=`date +'%Y%m%d_%H%M%S'`
	GZ_FILENAME="qo_onboard_samples__elf_amd64_${CMAKE_BUILD_PLATFORM}__release_${now}.tar.gz"
	pushd ${PACKAGE_DIR}
	tar -cz -f "${GZ_FILENAME}" --exclude=*.gz *
	popd

	if "${PACKAGE_LIBRARY_BINARIES}"
	then
		rm -rf ${PACKAGE_DIR}/lib/
		rm -rf ${PACKAGE_DIR}/include/
		rm -rf ${PACKAGE_DIR}/documentation/
	fi

	rm ${PACKAGE_DIR}/Config.md
	rm ${PACKAGE_DIR}/README.md
	rm ${PACKAGE_DIR}/qo_onboard_sample_code_A_using_config_file.c
	rm ${PACKAGE_DIR}/qo_onboard_sample_code_B_using_setopt_api.c
	rm ${PACKAGE_DIR}/qo_onboard_sample_code_C_minimal_get32bytes.c
	rm ${PACKAGE_DIR}/qo_onboard_sample_code_D_minimal_coinflip.c
	rm ${PACKAGE_DIR}/qo_onboard_sample_code_seed.h
	rm ${PACKAGE_DIR}/qo_onboard_sample_config.yml
	rm ${PACKAGE_DIR}/Makefile
	rm ${PACKAGE_DIR}/qoo_samples.sh
	rm ${PACKAGE_DIR}/qoo_samples_build.sh
	rm ${PACKAGE_DIR}/qoo_samples_run.sh

	# Note that we leave the "qoo_samples_install.sh" script alongside the tarball. This is what the user can/should use to install the samples.
	#rm ${PACKAGE_DIR}/qoo_samples_install.sh

	# The default target directory is "${HOME}/quantum-origin-onboard"
	# Once installed, the user can/should run "qoo_samples_build.sh" and then "qoo_samples_run.sh".

	if "${PACKAGE_SAMPLE_BINARIES}"
	then
		rm -rf ${PACKAGE_DIR}/bin/
	fi

	echo -e "--- fnSampleCodeCreateTarball - END"
}

function fnPackageSampleCode
{
	fnGetDistro
	CMAKE_BUILD_PLATFORM=${OS_DISTRO_NAME}_${OS_DISTRO_RELEASE} # e.g. "Ubuntu_20.04" or "CentOS_7.9.2009"
	CMAKE_TARGET_PLATFORM=${OS_DISTRO_NAME}_${OS_DISTRO_RELEASE} # e.g. "Ubuntu_20.04" or "CentOS_7.9.2009" or "Windows_10.0"
	echo "CMAKE_BUILD_PLATFORM  = ${CMAKE_BUILD_PLATFORM}"
	echo "CMAKE_TARGET_PLATFORM = ${CMAKE_TARGET_PLATFORM}"

	#CMAKE_BUILD_TYPE=Debug
	CMAKE_BUILD_TYPE=Release
	echo "CMAKE_BUILD_TYPE      = ${CMAKE_BUILD_TYPE}"

	echo -e "--- [fnBuildAndTest] PublishTarballPackage..."
	fnSampleCodeCreateTarball . ./build/package_sample_code true false false
}

function fnBuildAndRunSampleCode() # DeveloperTrack
{
	echo "${BOLD1}*** Package, install, build and run sample code${CLEAR}"

	echo "${BOLD1}*** Decompress tarball to INSTALL_TARGET - typically ~/quantum-origin-onboard${CLEAR}"
	rm -rf ./build/package_sample_code/
	fnPackageSampleCode

	echo "${BOLD1}*** Unzip tarball to INSTALL_TARGET - typically ~/quantum-origin-onboard${CLEAR}"
	pushd ./build/package_sample_code
	rm -rf ~/quantum-origin-onboard/
	./qoo_samples_install.sh
	popd

	echo "${BOLD1}*** Build sample code${CLEAR}"
	pushd ~/quantum-origin-onboard/
	rm -f ./*.o ./*.out
	rm -rf ${INSTALL_TARGET}
	./qoo_samples_build.sh
	popd

	echo "${BOLD1}*** Running sample code${CLEAR}"
	pushd ~/quantum-origin-onboard/
	./qoo_samples_run.sh
	popd

	echo "${BOLD1}*** Done${CLEAR}"
}

#########################################
## Sample code section - END
#########################################

###################################
# help
###################################
function fnHelp()
{
	echo "${BOLD1}**********************************************${CLEAR}"
	echo "${BOLD1}*** Project: QuantumOrigin.Library.Onboard${CLEAR}"
	echo "${BOLD1}*** File   : build_and_deploy.sh${CLEAR}"
	echo "${BOLD1}*** Usage  : build_and_deploy.sh [arg]${CLEAR}"
	echo "${BOLD1}*** arg    : help${CLEAR}"
	echo "${BOLD1}***          install_clang15 | install_deps | install_cross_deps${CLEAR}"
	echo "${BOLD1}***          build | quickbuild | build_aarch64 | build_armhf | build_windows${CLEAR}"
	echo "${BOLD1}***          build_release | build_aarch64_release | build_armhf_release | build_windows_release${CLEAR}"
	echo "${BOLD1}***          publish | publish_windows${CLEAR}"
	echo "${BOLD1}***          tests | simpleclitests | sample_code${CLEAR}"
	echo "${BOLD1}***          clean | clean_all${CLEAR}"
	echo "${BOLD1}**********************************************${CLEAR}"
}


###################################
# main
###################################
PROJECTNAME="[QOOnboard]"
TARGET_NAME=qoonboard

echo "${BOLD3}*** $0 : ${PROJECTNAME} build script ***${CLEAR}"

if [[ "$1" = "help" ]]                                               ; then fnHelp
elif [[ "$1" = "install_deps" || "$1" = "installdeps" ]]             ; then fnInstallDependencies                                   ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "install_clang15" ]]                                  ; then fnGetClang15                                            ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "quickbuild" ]]                                       ; then fnQuickBuild "Debug" "default" "${2}"                   ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "build" ]]                                            ; then fnBuild "Debug" "default" "${2}"                        ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "rebuild" ]]                                          ; then fnRebuild "Debug" "default" "${2}"                      ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "build_release" ]]                                    ; then fnBuild "Release" "default" "${2}"                      ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "publish" ]]                                          ; then fnPublish                                               ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "publish_windows" ]]                                  ; then fnPublishWindows                                        ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "sample_code" || "$1" = "samplecode" ]]               ; then fnBuildAndRunSampleCode                                 ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "build_windows" ]]                                    ; then fnBuild "Debug" ./profiles/windows_x64.profile "${2}"   ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "build_windows_release" ]]                            ; then fnBuild "Release" ./profiles/windows_x64.profile "${2}" ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "tests" ]]                                            ; then fnTests                                                 ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "simpleclitests" ]]                                   ; then fnSimpleCLITests                                        ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "clean" ]]                                            ; then fnClean                                                 ; echo -e "\e[96m--- Done\e[0m"
elif [[ "$1" = "clean_all" || "$1" = "cleanall" ]]                   ; then fnCleanAll                                              ; echo -e "\e[96m--- Done\e[0m"
else
	# Backwards-compatibility with original script
	fnBuild "Debug" "default" "${1}"
fi
