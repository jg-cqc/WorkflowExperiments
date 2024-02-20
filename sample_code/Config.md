# Configuration

The Quantum Origin Onboard system requires several configuration parameters to be specified in order to function correctly. These parameters can be supplied in the form of YAML format configuration file, or can be supplied programmatically prior to a request for randomness.

This document refers primarily to the configuration file variant of the API, but the details are applicable equally to the programmatic 'setopt' API.

There are four configuration sections needed:

* Logging
* Cache
* Weak-source of randomness
* Seed

These are covered in detail below, as well as the error codes that could be returned if the configuration is invalid or incomplete.

## Configuration Definition

The full definition is as follows...

```
wsr:
  type: FILE | RDSEED
  <type = file> path: [> Path to the weak-source of randomness file]

cache:
  type: [none | caching]
  <type != none> size: [> Size of the cache, in bytes]
  <type != none> prefill: [> Amount of bytes to generate at startup]
  <type != none> refill_at: [> The minimum number of bytes it will try and keep in the cache]

logging:
  mode: [daily_file | file | stderr | stdout | syslog]
  level: [trace | debug | info | warn | err | critical | off]
  <mode = file> path: [> The file to log to]
  <mode = daily_logger> path: [> The directory to log to]

seed:
  signature: [> The seed signature, base64-encoded]
  content: [> The seed, base64-encoded]
```

Further information and detail on each option can be found below.


### Logging

There are six modes available for logging, which in turn set the default logger for spdlog accordingly.

* ``stdout`` - Use stdout for logging.
* ``stderr`` - Use stderr for logging
* ``syslog`` - Use syslog as per local system for logging.  Ensure syslog is enabled in the kernel.
* ``file`` - Log to a file.  A ``path`` parameter is required to define which file to log to.
* ``daily_file`` - Log to a new file every day.  A ``path`` parameter is required to define which files to log to.

Furthermore, the default level of logging can be specified:

* ``trace | debug | info | warn | err | critical | off``.

### Cache

The Core randomness library generates blocks of entropy until sufficient entropy has been created.  More entropy than immediately required may be generated.  By using caching, the excess entropy for one request can be used in a later request.  Disable caching to generate immediately fresh entropy on all occasions.

The block sizes are: X and Y bytes.

The options for the cache are:

* ``none``: No caching, always generate fresh entropy for every request.  This may be slower for many small requests.
* ``caching``: Retain excess entropy and use for later requests. A new thread is started that will automatically ensure the cache is kept at a specified size.

For caching modes, three further parameters are required

* ``size``: The number of bytes that the cache should keep as a maximum
* ``prefill``: The number of bytes to insert into the cache upon construction
* ``refill_at``: The low point at which to refill the cache.

### Weak-source of randomness (WSR)

There are currently two integrations for a weak-source of randomness.

This is a user-provided source of randomness that is used as an input to the Core Randomness generation library.

Options:

* ``type``: The type of WSR, ``rdseed`` or ``file``.

If the WSR is a file, a further ``path`` parameter must be provided, e.g. ``/dev/hwrng``.

### Seed

A seed must be provided for the library to work.

Two parameters must be provided:

* ``content``:  The content of a seed.
* ``signature``:  A signed hash provided by Quantinuum to the customer.

## Error Codes

Below is a list of the potential error codes, and their associated descriptions, that could be returned by Quantum Origin Onboard API:

| Name                                            | Description                                                         |
|-------------------------------------------------|---------------------------------------------------------------------|
|ERC_QOOBR_EPARAM_CONFIG_FILENAME_NOT_SUPPLIED    | A required configuration filename parameter was not supplied        |
|ERC_QOOBR_EPARAM_NODE_PATH_NOT_SUPPLIED          | The required node-path parameter was not supplied                   |
|ERC_QOOBR_EPARAM_CONFIG_PTR_NOT_SUPPLIED         | The required configuration pointer parameter was not supplied       |
|ERC_QOOBR_EPARAM_ONBOARD_CONTEXT_NOT_SUPPLIED    | The required context pointer parameter was not supplied             |
|ERC_QOOBR_EPARAM_DEST_BUFFER_NOT_SUPPLIED        | The required destination buffer output parameter was not supplied   |
|ERC_QOOBR_EPARAM_BYTES_RETURNED_PTR_NOT_SUPPLIED | The required byte-count output parameter was not supplied           |
|ERC_QOOBR_EPARAM_VALUE_PTR_NOT_SUPPLIED          | The required config value parameter was not supplied                |
|ERC_QOOBR_FAILED_TO_ASSIGN_SEED_SIGNATURE        | Setting the seed signature failed                                   |
|ERC_QOOBR_FAILED_TO_ASSIGN_SEED_CONTENT          | Setting the seed content failed                                     |
|ERC_QOOBR_UNSUPPORTED_OPTION                     | The attempted action is not supported                               |
|ERC_QOOBR_ONBOARD_STD_EXCEPTION                  | A standard exception occurred, see error string for more details    |
|ERC_QOOBR_ONBOARD_UNKNOWN_EXCEPTION              | An unknown exception occurred                                       |
|ERC_QOOBR_EPARAM_CALLBACK_PTR_NOT_SUPPLIED       | The required callback function parameter was not supplied           |
|ERC_QOOBR_EPARAM_VSNPRINTF                       | An error occured whilst using vsnprintf                             |
|ERC_QOOBR_EPARAM_MSG_BUFFER_NOT_SUPPLIED         | The required message buffer was not supplied                        |
|ERC_QOOBR_FAILED_TO_ASSIGN_LICENSE_CONTENT       | Setting the license content failed                                  |

These are defined in the header file `qo_onboard_c.h`.
