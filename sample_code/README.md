# Quantum Origin Onboard - Sample Code

This folder holds some sample code and associated scripts which demonstrate how to interface programmatically with the QO-Onboard C Library


## Files

*Documentation*

* README.md
This file.

* Config.md
Document covering the features and specifics of a configuration file.

*Samples*

There are four sample programs, each of which interfaces to the Quantum Origin Onboard C API.

* qo_onboard_sample_code_A_using_config_file.c
Standalone program that requests a block of randomness from the library, while also making use of the logging features. Parameters and configuration are supplied to the library via a config file: `qo_onboard_sample_config.ymlqo_onboard_sample_config.yml`.
* qo_onboard_sample_code_B_using_setopt_api.c
Standalone program similar to the above, but supplies the configuration parameters directly to the library using the setopt functions, with no need to access a file-system.
* qo_onboard_sample_code_C_minimal_get32bytes.c
Minimal application that retrieves a small amount of randomness
* qo_onboard_sample_code_D_minimal_coinflip.c
Simplistic application that retrieves a small amount of randomness and then prints "HEADS" or "TAILS" based on the value of a single bit.

*Supporting and Utility files*

* qo_onboard_sample_code_seed.h
This "header" contains the definition of a sample seed. This is used by some of the samples mentioned above.

* qo_onboard_sample_config.yml
A Sample configuration file. This file is also used by some of the samples mentioned above.

*Build files and scripts*
* Makefile
* qoo_samples.sh
* qoo_samples_build.sh
* qoo_samples_install.sh
* qoo_samples_run.sh


## Building

```
make [install | build | buildall | rebuild | rebuildall (default) | run | clean | cleanall
```

Or alternatively:

```
./qoo_samples.sh [ help | install | build | buildall | run | clean | cleanall ]
```
The default action of ./qoo_samples.sh, when run with no arg is effectively a `rebuildall`: clean, install, buildall in succession.
