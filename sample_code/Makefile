# Minimal makefile for sample code
#

.PHONY: all
all: rebuildall

.PHONY: install
install:
	./qoo_samples.sh install

.PHONY: build
build:
	./qoo_samples.sh build

.PHONY: buildall
buildall:
	./qoo_samples.sh buildall

.PHONY: run
run:
	./qoo_samples.sh run

.PHONY: clean
clean:
	./qoo_samples.sh clean

.PHONY: cleanall
cleanall:
	./qoo_samples.sh cleanall

.PHONY: rebuild
rebuild: clean build

.PHONY: rebuildall
rebuildall: cleanall install buildall


# Aliases with Underscores
.PHONY: build_all
build_all: buildall

.PHONY: rebuild_all
rebuild_all: rebuildall

.PHONY: clean_all
clean_all: cleanall
