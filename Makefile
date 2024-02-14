#.ONESHELL:
SHELL = /bin/bash
#.SHELLFLAGS = -e

PROJECTNAME = "[QuantumOrigin.Library.Onboard]"

.PHONY: all
all: build
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: installdeps
installdeps:
	./build_and_install.sh installdeps
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: quickbuild
quickbuild:
	./build_and_install.sh quickbuild
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: build
build:
	./build_and_install.sh build
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: build_cmake_package
build_cmake_package:
	./build_and_install.sh build_cmake_package
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: package
package: build_cmake_package

.PHONY: rebuild
rebuild:
	./build_and_install.sh rebuild
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: build_release
build_release:
	./build_and_install.sh build_release
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: buildrelease
buildrelease: build_release

.PHONY: build_windows
build_windows:
	./build_and_install.sh build_windows
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: buildwindows
buildwindows: build_windows

.PHONY: build_windows_release
build_windows_release:
	./build_and_install.sh build_windows_release
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: build_windowsrelease
buildwindows_release: build_windows_release

.PHONY: publish
publish:
	./build_and_install.sh publish
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: publish_windows
publish_windows:
	./build_and_install.sh publish_windows
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: publishwindows
publishwindows: publish_windows

.PHONY: sample_code
sample_code:
	./build_and_install.sh sample_code
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: samplecode
samplecode: sample_code

.PHONY: tests
tests:
	./build_and_install.sh tests
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: clean
clean:
	./build_and_install.sh clean
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: test
test: tests

.PHONY: simpleclitests
simpleclitests:
	./build_and_install.sh simpleclitests
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: clean_all
clean_all:
	./build_and_install.sh clean_all
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: cleanall
cleanall: clean_all

.PHONY: help
help:
	./build_and_install.sh help
	@echo "*** $(PROJECTNAME) $@ Done"

.PHONY: usage
usage: help

.PHONY: install_deps
install_deps: installdeps
