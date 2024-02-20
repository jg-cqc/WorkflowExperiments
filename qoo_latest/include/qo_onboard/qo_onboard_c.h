/**
 * @file qo_onboard_c.h
 *
 * Quantum Origin Onboard C-Style API
 *
 * Quantum Origin Onboard is an entropy-enhancement engine, combining a Quantinuum-generated quantum seed with an end user's weak source of randomness to create near-perfect randomness.
 * Quantum Origin Onboard forms the core of multiple Quantinuum and Quantum Origin products, as well as being available as a standalone, offline library with a C++ or C interface.
 *
 * This header file defines the API of this C-Interface.
 */

#pragma once

#include <stddef.h>

#ifndef ERC_OK
#define ERC_OK 0 //!< Operation completed successfully
#endif
#ifndef ERC_UNSPECIFIED_ERROR
#define ERC_UNSPECIFIED_ERROR 19999 //!< An unspecified error occurred
#endif
#define ERC_QOOBR_FLOOR                                  13800 //!< Base value for error codes used by the Quantum Origin Onboard C-Style API
#define ERC_QOOBR_EPARAM_CONFIG_FILENAME_NOT_SUPPLIED    13801 //!< A required configuration filename parameter was not supplied
#define ERC_QOOBR_EPARAM_NODE_PATH_NOT_SUPPLIED          13802 //!< The required node-path parameter was not supplied
#define ERC_QOOBR_EPARAM_CONFIG_PTR_NOT_SUPPLIED         13803 //!< The required configuration pointer parameter was not supplied
#define ERC_QOOBR_EPARAM_ONBOARD_CONTEXT_NOT_SUPPLIED    13804 //!< The required context pointer parameter was not supplied
#define ERC_QOOBR_EPARAM_DEST_BUFFER_NOT_SUPPLIED        13805 //!< The required destination buffer output parameter was not supplied
#define ERC_QOOBR_EPARAM_BYTES_RETURNED_PTR_NOT_SUPPLIED 13806 //!< The required byte-count output parameter was not supplied
#define ERC_QOOBR_EPARAM_VALUE_PTR_NOT_SUPPLIED          13807 //!< The required config value parameter was not supplied
#define ERC_QOOBR_FAILED_TO_ASSIGN_SEED_SIGNATURE        13808 //!< Setting the seed signature failed
#define ERC_QOOBR_FAILED_TO_ASSIGN_SEED_CONTENT          13809 //!< Setting the seed content failed
#define ERC_QOOBR_UNSUPPORTED_OPTION                     13810 //!< The attempted action is not supported
#define ERC_QOOBR_ONBOARD_STD_EXCEPTION                  13811 //!< A standard exception occurred, see error string for more details
#define ERC_QOOBR_ONBOARD_UNKNOWN_EXCEPTION              13812 //!< An unknown exception occurred
#define ERC_QOOBR_EPARAM_CALLBACK_PTR_NOT_SUPPLIED       13813 //!< The required callback function parameter was not supplied
#define ERC_QOOBR_EPARAM_VSNPRINTF                       13814 //!< An error occured whilst using vsnprintf
#define ERC_QOOBR_EPARAM_MSG_BUFFER_NOT_SUPPLIED         13815 //!< The required message buffer was not supplied
#define ERC_QOOBR_FAILED_TO_ASSIGN_LICENSE_CONTENT       13816 //!< Setting the license content failed

#ifdef __cplusplus
extern "C"
{
#endif

    ///////////////////////////////////////////////////////////////////////////////
    // Note that all enum entries here are given explicit values so that, even if
    // one gets deprecated or removed, then the external, customer-facing API
    // remains unchanged.
    ///////////////////////////////////////////////////////////////////////////////


    /**
     * @enum tONBOARD_WSRTYPE
     * @brief Enum of the supported WSR types as passed to onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_WSR_TYPE, ONBOARD_WSRTYPE_RDSEED );
     *
     * Quantum Origin works by combining two sources of randomness in a very particular way - sometimes called amplification.
     * The primary source of randomness being fed into the Quantum Origin extractor is a Quantum seed. A WSR (Weak Source of Randomness) is the name given to the secondary source of randomness.
     * The term "weak", as used here, is relative - specifically relative to the near-perfect randomness produced from a Quantum process.
     *
     * The desired second source of randomness can be set using a call to onboard_setopt_int(), specifying
     * the ONBOARD_OPT_WSR_TYPE option and the appropriate type selected from this enum.
     * Some types may require additional parameters as mentioned below.
     *
     * For example:
     * @code
     *     ...
     *     tONBOARD_CONFIG *pOnboardConfig = NULL;
     *     tQO_ONBOARD_RESULT erc = ERC_UNSPECIFIED_ERROR;
     *
     *     pOnboardConfig = onboard_setopt_init();
     *     ASSERT_NE(pOnboardConfig, NULL)
     *     ...
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_WSR_TYPE, ONBOARD_WSRTYPE_RDSEED);
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     ...
     *     pOnboard = onboard_init_from_config_struct(pOnboardConfig);
     *     if (pOnboard == NULL) { Report and process error as needed }
     *
     *     // The config structure is no longer needed, and can be destroyed.
     *     onboard_setopt_cleanup(pOnboardConfig);
     *     pOnboardConfig = NULL;
     *     ...
     * @endcode
     */
    typedef enum tONBOARD_WSRTYPE
    {
        ONBOARD_WSRTYPE_RDSEED            = 0,  //!< Use weak randomness obtained from Intel's RDSEED instruction
        ONBOARD_WSRTYPE_FILE              = 1,  //!< Read weak randomness from a file (which can be a device such as /dev/random).  The filename itself needs to be supplied using a call to onboard_setopt_str() with parameter ONBOARD_OPT_LOGGING_FILENAME.
        ONBOARD_WSRTYPE_CALLBACK_FUNCTION = 2,  //!< Obtain weak randomness by calling a callback function.  A ptr to a callback function of type tQO_RNG_GET_WSR_DATA_FN needs to be supplied using the onboard_setopt_wsr_callback() function.
    } tONBOARD_WSRTYPE;


    /**
     * @typedef OnboardWSRType
     * @see tONBOARD_WSRTYPE
     */
    typedef tONBOARD_WSRTYPE OnboardWSRType;

    /**
     * @enum tONBOARD_CACHETYPE
     * @brief Enum of the available mechanisms available for the internal caching of randomness.
     *
     * Enabling the internal caching of randomness can have a significant positive effect on performance, but may have some
     * security implications in some situations.
     * The Quatum Origin randomness extractor produces, on demand, randomness in blocks. It is common that, after providing the
     * amount of randomness requested, there are some bytes remaining. These could be discarded (No caching) or held in memory
     * and be available for, or towards, a subsequent request.
     * The potential down-side of this is that the randomness supplied in a second or subsequent call could potentially have
     * been in memory since a previous request.
     *
     * The desired caching mechanism can be set using a call to onboard_setopt_int(), specifying
     * the ONBOARD_OPT_CACHE_TYPE option and the appropriate type selected from this enum.
     *
     * For example:
     * @code
     *     ...
     *     tONBOARD_CONFIG *pOnboardConfig = NULL;
     *     tQO_ONBOARD_RESULT erc = ERC_UNSPECIFIED_ERROR;
     *
     *     pOnboardConfig = onboard_setopt_init();
     *     ASSERT_NE(pOnboardConfig, NULL)
     *     ...
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_CACHE_TYPE, ONBOARD_CACHETYPE_NONE);
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     ...
     *     pOnboard = onboard_init_from_config_struct(pOnboardConfig);
     *     if (pOnboard == NULL) { Report and process error as needed }
     *
     *     // The config structure is no longer needed, and can be destroyed.
     *     onboard_setopt_cleanup(pOnboardConfig);
     *     pOnboardConfig = NULL;
     *     ...
     * @endcode
     */
    typedef enum tONBOARD_CACHETYPE
    {
        ONBOARD_CACHETYPE_NONE        = 0,  //!< Disable caching of randomness
        ONBOARD_CACHETYPE_CACHING     = 1,  //!< Cache randomness using a cache filled asynchronously by a background thread
        ONBOARD_CACHETYPE_SYNCCACHING = 2,  //!< Cache randomness uses a cache that is filled synchronously when a randomness request is made
        ONBOARD_CACHETYPE_MULTITHREAD = 3,  //!< Cache randomness using a cache filled asynchronously by multiple background threads
    } tONBOARD_CACHETYPE;


    /**
     * @typedef OnboardCacheType
     * @see tONBOARD_CACHETYPE
     */
    typedef tONBOARD_CACHETYPE OnboardCacheType;

    /**
     * @enum tONBOARD_LOGMODE
     * @brief Enum of the supported logging modes i.e. the location or subsystem to where log entries are written.
     *
     * The system outputs various log entries as determined by the configured log level.
     * These log entries are written to a logging "destination".
     * The supported destinations include the console (stdout or stderr), the operating
     * system's internal logging subsystem (syslog), a file (file or dailyfile) or to simply
     * inherit the setting from a parent's instance of spdlog settings.
     *
     * The desired mode of logging can be set using a call to onboard_setopt_int(), specifying
     * the ONBOARD_OPT_LOGGING_MODE option and the appropriate mode selected from this enum.
     * Some modes may require additional parameters as mentioned below.

     * @see tONBOARD_LOGLEVEL
     *
     * For example:
     * @code
     *     ...
     *     tONBOARD_CONFIG *pOnboardConfig = NULL;
     *     tQO_ONBOARD_RESULT erc = ERC_UNSPECIFIED_ERROR;
     *
     *     pOnboardConfig = onboard_setopt_init();
     *     ASSERT_NE(pOnboardConfig, NULL)
     *     ...
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_LOGGING_MODE, ONBOARD_LOGMODE_DAILYFILE);
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_LOGGING_FILENAME, "mylogfile.log");
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     ...
     *     pOnboard = onboard_init_from_config_struct(pOnboardConfig);
     *     if (pOnboard == NULL) { Report and process error as needed }
     *
     *     // The config structure is no longer needed, and can be destroyed.
     *     onboard_setopt_cleanup(pOnboardConfig);
     *     pOnboardConfig = NULL;
     *     ...
     * @endcode
     */
    typedef enum tONBOARD_LOGMODE
    {
        ONBOARD_LOGMODE_STDOUT    = 0,  //!< Send logging to the console via stdout.
        ONBOARD_LOGMODE_STDERR    = 1,  //!< Send logging to the console via stderr.
        ONBOARD_LOGMODE_SYSLOG    = 2,  //!< Send logging to syslog.
        ONBOARD_LOGMODE_DAILYFILE = 3,  //!< Send logging to a file, rotating the file each day.  The base name of the logfile needs to be supplied using ONBOARD_OPT_LOGGING_FILENAME.
        ONBOARD_LOGMODE_FILE      = 4,  //!< Send logging to a file.  The name of the logfile needs to be supplied using ONBOARD_OPT_LOGGING_FILENAME.
        ONBOARD_LOGMODE_INHERIT   = 5,  //!< Inherit logging settings from parent application (will use whichever logger is configured as spdlog default)
#ifdef _WIN32
        ONBOARD_LOGMODE_WINEVTLOG = 6,  //!< Send logging to the Windows EventLog
#endif
    } tONBOARD_LOGMODE;
    /**
     * @typedef OnboardLoggingMode
     * @see tONBOARD_LOGMODE
     */
    typedef tONBOARD_LOGMODE OnboardLoggingMode;


    /**
     * @enum tONBOARD_LOGLEVEL
     * @brief Enum of the supported logging levels, which are used to control the amount of logging detail that is output.
     *
     * The system outputs various log entries which are categorised into "levels".
     * These levels range from Critical, through Error, Warning, Informative and debugging.
     * The desired level of output can be set using a call to onboard_setopt_int(), specifying
     * the ONBOARD_OPT_LOGGING_LEVEL option and the appropriate level selected from this enum.
     *
     * @see tONBOARD_LOGMODE
     *
     * For example:
     * @code
     *     ...
     *     tONBOARD_CONFIG *pOnboardConfig = NULL;
     *     tQO_ONBOARD_RESULT erc = ERC_UNSPECIFIED_ERROR;
     *
     *     pOnboardConfig = onboard_setopt_init();
     *     ASSERT_NE(pOnboardConfig, NULL)
     *     ...
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_LOGGING_LEVEL, ONBOARD_LOGLEVEL_WARNING);
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     ...
     *     pOnboard = onboard_init_from_config_struct(pOnboardConfig);
     *     if (pOnboard == NULL) { Report and process error as needed }
     *
     *     // The config structure is no longer needed, and can be destroyed.
     *     onboard_setopt_cleanup(pOnboardConfig);
     *     pOnboardConfig = NULL;
     *     ...
     * @endcode
     */
    typedef enum tONBOARD_LOGLEVEL
    {
        ONBOARD_LOGLEVEL_NONE     = 0,  //!< No logging
        ONBOARD_LOGLEVEL_OFF      = 0,  //!< No logging  (same as ONBOARD_LOGLEVEL_NONE)
        ONBOARD_LOGLEVEL_CRITICAL = 1,  //!< Log only critical errors
        ONBOARD_LOGLEVEL_CRIT     = 1,  //!< Log only critical errors (same as ONBOARD_LOGLEVEL_CRITICAL)
        ONBOARD_LOGLEVEL_ERROR    = 2,  //!< Log all errors, including critical errors
        ONBOARD_LOGLEVEL_ERR      = 2,  //!< Log all errors (same as ONBOARD_LOGLEVEL_ERROR)
        ONBOARD_LOGLEVEL_WARNING  = 3,  //!< Log warnings, errors and critical errors
        ONBOARD_LOGLEVEL_WARN     = 3,  //!< Log warnings (same as ONBOARD_LOGLEVEL_WARNING)
        ONBOARD_LOGLEVEL_INFO     = 4,  //!< Log informative messages, warnings, errors and critical errors
        ONBOARD_LOGLEVEL_DEBUG    = 5,  //!< Log debug messages, as well as all info, warning and error messages
        ONBOARD_LOGLEVEL_TRACE    = 6,  //!< Log detailed trace messages, as well as all debug messages, warnings and errors
    } tONBOARD_LOGLEVEL;


    /**
     * @typedef OnboardLoggingVerbosity
     * @see tONBOARD_LOGLEVEL
     */
    typedef tONBOARD_LOGLEVEL OnboardLoggingVerbosity;


    /**
     * @enum tONBOARD_OPTID
     * @brief Enum of supported Option IDs which are used to programmatically configure Quantum Origin Onboard.
     *
     * Quantum Origin Onboard can be configured in various ways, including via a config file on disk, and via options
     * specified programmatically by an application using the onboard_setopt_xxx set of functions.
     * The values in this enum are used to specify which of the various settings to change programmatically.
     * Typically, an application will initialise an onboard context with a call to onboard_setopt_init(), and then
     * call one or more of the onboard_setopt_[string|int|bytes]() functions with the appropriate Option ID to
     * configure each particular setting.
     *
     * For example:
     * @code
     *     ...
     *     tONBOARD_CONFIG *pOnboardConfig = NULL;
     *     tQO_ONBOARD_RESULT erc = ERC_UNSPECIFIED_ERROR;
     *
     *     pOnboardConfig = onboard_setopt_init();
     *     ASSERT_NE(pOnboardConfig, NULL)
     *     ...
     *     erc = onboard_setopt_str(pOnboardConfig, ONBOARD_OPT_LOGGING_FILENAME, "mylogfile.log");
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     erc = onboard_setopt_int(pOnboardConfig, ONBOARD_OPT_CACHE_SIZE, 2048);
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     erc = onboard_setopt_bytes(pOnboardConfig, ONBOARD_OPT_SEED_SIGNATURE, seedSignature.data(), seedSignature.size());
     *     if (erc != ERC_OK) { Report and process error as needed }
     *     ...
     *     pOnboard = onboard_init_from_config_struct(pOnboardConfig);
     *     if (pOnboard == NULL) { Report and process error as needed }
     *
     *     // The config structure is no longer needed, and can be destroyed.
     *     onboard_setopt_cleanup(pOnboardConfig);
     *     pOnboardConfig = NULL;
     *     ...
     * @endcode
     */
    typedef enum tONBOARD_OPTID
    {
        ONBOARD_OPT_LOGGING_FILENAME    = 0,  //!< String. The fully qualified logging filename.
        ONBOARD_OPT_LOGGING_LEVEL       = 1,  //!< Numeric. One of the values in the tONBOARD_LOGLEVEL enum.
        ONBOARD_OPT_LOGGING_MODE        = 2,  //!< Numeric. One of the values in the tONBOARD_LOGMODE enum.
        ONBOARD_OPT_CACHE_TYPE          = 3,  //!< Numeric. One of the values in the tONBOARD_CACHETYPE enum.
        ONBOARD_OPT_CACHE_SIZE          = 4,  //!< Numeric. The size of the cache in bytes.
        ONBOARD_OPT_CACHE_PREFILL       = 5,  //!< Numeric. The "high water mark" of the cache. The loading of the cache stops when the number of bytes exceeds this value.
        ONBOARD_OPT_CACHE_REFILL_AT     = 6,  //!< Numeric. The "low water mark" of the cache. The loading of the cache resumes when the number of bytes falls below this value.
        ONBOARD_OPT_WSR_TYPE            = 7,  //!< Numeric. The type of WSR used. The value is one of the values from the tONBOARD_WSRTYPE enum.
        ONBOARD_OPT_WSR_PATH            = 8,  //!< String. The path to the WSR source.
        ONBOARD_OPT_HEALTH_TESTS_OUTPUT = 9,  //!< Numeric (Boolean). Whether or not the health tests are run.
        ONBOARD_OPT_SEED_SIGNATURE      = 10, //!< Byte array containing the seed signature.
        ONBOARD_OPT_SEED_CONTENT        = 11, //!< Byte Array containing the seed itself.
        ONBOARD_OPT_CACHE_THREAD_COUNT  = 12, //!< Numeric. The number of threads servicing the cache.
        ONBOARD_OPT_LICENSE_DATA        = 13, //!< Byte array containing the license data.
    } tONBOARD_OPTID;

    /**
     * @typedef ONBOARD_OPT_ID
     * @see tONBOARD_OPTID
     */
    typedef tONBOARD_OPTID ONBOARD_OPT_ID;


    typedef enum tQO_WSR_CB_ERRCODE
    {
        QO_WSR_CB_SUCCESS                                     = 0,     //!< Operation completed successfully
        QO_WSR_CB_FLOOR                                       = 41000, //!< Base value for error codes returned by a user-define WSR callback function
        QO_WSR_CB_RESULT_ENOMEM_OUT_OF_MEMORY                 = 41010, //!< ERROR: Memory allocation for object failed
        QO_WSR_CB_RESULT_EPARAM_NO_BUFFER_SUPPLIED            = 41020, //!< ERROR: Ptr to destination buffer is NULL
        QO_WSR_CB_RESULT_EPARAM_NO_USERDATA                   = 41021, //!< ERROR: Ptr to UserData is NULL
        QO_WSR_CB_RESULT_EINVAL_INVALID_SIZE                  = 41022, //!< ERROR: Invalid size of object
        QO_WSR_CB_RESULT_EINVAL_INVALID_FUNCTIONPTR           = 41023, //!< ERROR: Function pointer is NULL
        QO_WSR_CB_RESULT_ERANGE_OUT_OF_RANGE                  = 41024, //!< ERROR: Value is out of range
        QO_WSR_CB_RESULT_ENOINIT_NOT_INITIALISED              = 41030, //!< ERROR: Subsystem/device is not initialized
        QO_WSR_CB_RESULT_FAILED_TO_OPEN_RANDOM_SOURCE         = 41040, //!< ERROR: Failed to open randomness source
        QO_WSR_CB_RESULT_FAILED_TO_READ_RANDOM_SOURCE         = 41045, //!< ERROR: Failed to read from randomness source
        QO_WSR_CB_RESULT_FAILED_TO_RETRIEVE_WSR_MATERIAL      = 41050, //!< ERROR: Failed to retrieve WSR material
        QO_WSR_CB_RESULT_UNSPECIFIED_ERROR                    = 41099  //!< ERROR: Unspecified error
    } tQO_WSR_CB_ERRCODE;

    /**
     * @typedef OnboardWsrCallbackErrorCode
     * @see tQO_WSR_CB_ERRCODE
     */
    typedef tQO_WSR_CB_ERRCODE OnboardWsrCallbackErrorCode;

    /**
     * tQO_ONBOARD_RESULT : Quantum Origin Onboard numeric return/error code.
     *
     * An enum of Quantum Origin Onboard numeric error codes as returned from various API functions.
     * For completeness, this enum includes an entry for 'success', namely ERC_OK, with value zero.
     * The most recent error code can also be retrieved with a call to onboard_get_error_code().
     * The onboard_get_error_description() function can be called to help determine the nature/cause of a specific failure.
     */
    typedef int tQO_ONBOARD_RESULT;

    /**
     * tQO_ONBOARD_CTX : Quantum Origin Onboard object/context.
     *
     * A pointer to a Quantum Origin Onboard object (tQO_ONBOARD_CTX *) is passed
     * to most API functions in order to identify the object context.
     */
    typedef void tQO_ONBOARD_CTX;

    /**
     * tONBOARD_CONFIG : A structure containing the active configuration values.
     *
     * The internals of this structure are subject to change without notice and thus should not be accessed directly.
     */
    typedef struct OnboardConfig tONBOARD_CONFIG;

    /**
     * tQO_RNG_GET_WSR_DATA_FN_PTR : A callback function for providing a weak source of randomness.
     *
     * Notes:
     *  1. This callback may be called from multiple threads, so should be thread safe.
     *  2. It is recommended that a forward declaration of the form...
     *         static tQO_RNG_GET_WSR_DATA_FN my_wsr_callback;
     *     ...is added to the top of the module in which the callback is implemented so
     *        as to ensure that the interface function is correctly defined.
     *
     * For example:
     * @code
     *     ...
     *     static tQO_RNG_GET_WSR_DATA_FN my_wsr_callback;
     *     static int my_wsr_callback(void *pUserData, unsigned char *pOutput, size_t outputLen)
     *     {
     *         // Store outputLen bytes of randomness to the buffer pointed to by pOutput.
     *         return ERC_OK;
     *     }
     *     ...
     * @endcode
     *
     * @param pUserData Pointer to arbitrary user-specified data.
     * @param pOutput   Pointer to the memory location that will receive the randomness.
     * @param outputLen The amount of randomness that is being requested.
     *
     * @return Must return ERC_OK (zero) on success, or any other value on failure.
     */
    typedef int (tQO_RNG_GET_WSR_DATA_FN)(void *pUserData, unsigned char *pOutput, size_t outputLen);
    typedef tQO_RNG_GET_WSR_DATA_FN *tQO_RNG_GET_WSR_DATA_FN_PTR;

    /**
     * tQO_LOGGER_CALLBACK_FN : A typedef of a logging callback function that will assist
     * with the correctness of the function definition and declaration.
     *
     * Notes:
     *  1. This callback may be called from multiple threads, so should be thread safe.
     *  2. It is recommended that a function forward declaration is added to the top of
     *     the module in which the callback is implemented so as to ensure that the
     *     interface function is correctly defined.
     *
     * For example:
     * @code
     *     ...
     *      // Forward declaration
     *     static tQO_LOGGER_CALLBACK_FN my_logger_callback;
     *
     *     // Implementation
     *     static void my_logger_callback(const tONBOARD_LOGLEVEL loglevel, const char *szMsg, size_t msgLen)
     *     {
     *         // Store or present the inbound message somewhere.
     *     }
     *     ...
     * @endcode
     *
     * @param loglevel  The severity of the log entry. The value is one of those in tONBOARD_LOGLEVEL.
     * @param szMsg     The null-terminated error message.
     * @param msgLen    The length of the error message (excluding the null-terminator).
     *
     * @return void
     */
    typedef void (tQO_LOGGER_CALLBACK_FN)(const tONBOARD_LOGLEVEL loglevel, const char *szMsg, size_t msgLen);
    typedef tQO_LOGGER_CALLBACK_FN *tQO_LOGGER_CALLBACK_FN_PTR;

    /**
     * onboard_init_from_config_file() : Initialize a QO Onboard context from a config file.
     *
     * @param szConfigFilename A null terminated string specifying the relative or absolute path to the configuration file.
     * @param szNodePath       A null terminated string specifying the parent yaml node under which the configuration exists e.g. "", "/" or "generator".
     *
     * @return On success, returns a ptr to an Onboard context. This is used and required for various other calls to the Onboard API.
     *         On error, returns NULL. A call to onboard_get_error_description() and onboard_get_error_code() can be called to determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_CTX *onboard_init_from_config_file(const char *szConfigFilename, const char *szNodePath);

    /**
     * onboard_init_from_config_struct() : Initialize an Onboard context from a config struct.
     *
     * NOTE: All calls to the required setopt functions must be made prior to calling this function.
     *       Any subsequent calls to setopt functions after a call to onboard_init_from_config_struct() will have no effect.
     *
     * @param pOnboardConfig A ptr to an Onboard configuration context as returned from a call to onboard_setopt_init() and modified as required with calls to
     * any of onboard_setopt_int(), onboard_setopt_str() or onboard_setopt_bytes().
     *
     * @return On success, returns a pointer to an Onboard context.
     *         On error, returns NULL. A call to onboard_get_error_description() and onboard_get_error_code() can be called to determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_CTX *onboard_init_from_config_struct(const tONBOARD_CONFIG *pOnboardConfig);

    /**
     * onboard_setopt_init() : Initialize an Onboard config to be used in the Onboard "setopt" set-up calls.
     *
     * @return On success, a pointer an internal config context is returned.
     *         On error, returns NULL. The only cause for a potential failure is an out-of-memory condition.
     */
    extern tONBOARD_CONFIG *onboard_setopt_init(void);

    /**
     * onboard_setopt_bytes() : Set the value of a config option whose type is bytes_t (std::vector<std::byte>).
     *
     * This does a deep copy of the data pointed to by pValue to the underlying std:vector using the assign method.
     * Consequently, the data at pValue is not required after this call, and may be destroyed or reused.
     *
     * @param pOnboardConfig A struct containing values required in initialising an Onboard context
     * @param option         An ONBOARD_OPT_ID enum for a config option whose type is byte.
     * @param pValue         A ptr to the value to which the specified option is set.
     * @param cbValue        The number of bytes of the value.
     *
     * @return On success, returns ERC_OK.
     *         On error, returns an error code identifying the problem.
     *         The functions onboard_get_error_description() and onboard_get_error_code() can be called to help determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_RESULT onboard_setopt_bytes(tONBOARD_CONFIG *pOnboardConfig, tONBOARD_OPTID option, unsigned char *pValue, size_t cbValue);

    /**
     * onboard_setopt_str() : Set the value of a config option whose type is a string (char*).
     *
     * @param pOnboardConfig A struct containing values required in initialising an Onboard context
     * @param option         An ONBOARD_OPT_ID enum for a config option whose type is string.
     * @param szValue        A ptr to a null terminated string value to which the specified option is set.
     *
     * @return On success, returns ERC_OK.
     *         On error, returns an error code identifying the problem.
     *         The functions onboard_get_error_description() and onboard_get_error_code() can be called to help determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_RESULT onboard_setopt_str(tONBOARD_CONFIG *pOnboardConfig, tONBOARD_OPTID option, const char *szValue);

    /**
     * onboard_setopt_int() :  Set the value of a config option whose type is an int.
     *
     * @param pOnboardConfig A struct containing values required in initialising an Onboard context
     * @param option         An ONBOARD_OPT_ID enum for a config option whose type is int.
     * @param value          The value to which the specified option is set.
     *
     * @return On success, returns ERC_OK.
     *         On error, returns an error code identifying the problem.
     *         The functions onboard_get_error_description() and onboard_get_error_code() can be called to help determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_RESULT onboard_setopt_int(tONBOARD_CONFIG *pOnboardConfig, tONBOARD_OPTID option, size_t value);

    /**
     * onboard_setopt_wsr_callback() : Set the value of the WSR callback config option
     *
     * @param config    A struct containing values required in initialising an Onboard context
     * @param pFunction A pointer to the callback function which can supply the secondary source of randomness.
     * @param pUserData Point to arbitrary user-specified data (will be passed on to the callback function)
     *
     * @return On success, returns ERC_OK.
     *         On error, returns an error code identifying the problem.
     *         The functions onboard_get_error_description() and onboard_get_error_code() can be called to help determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_RESULT onboard_setopt_wsr_callback(tONBOARD_CONFIG *pOnboardConfig, tQO_RNG_GET_WSR_DATA_FN_PTR pFunction, void *pUserData);

    /**
     * onboard_setopt_cleanup() : Clean up any memory allocated dynamically during the easy setopt process
     *
     * @param pOnboardConfig Pointer to a config struct previously obtained from onboard_setopt_init()
     */
    extern void onboard_setopt_cleanup(tONBOARD_CONFIG *pOnboardConfig);

    /**
     * onboard_get_randomness() : Request randomness from the provided Onboard context.
     *
     * @param pOnboard        An initialized Onboard context.
     * @param pBuffer         A buffer of at least buf_size to be filled with random bytes.
     * @param cbBuffer        The size of pBuffer in bytes.
     * @param pBytesReturned  The number of bytes written to pBuffer on return.
     *
     * @return On success, returns ERC_OK (0)
     *         On error, returns a non-zero error code indicating the cause of the problem.
     *         The functions onboard_get_error_description() and onboard_get_error_code() can be called to help determine the nature/cause of the failure.
     */
    extern tQO_ONBOARD_RESULT onboard_get_randomness(tQO_ONBOARD_CTX *pOnboard, unsigned char *pBuffer, size_t cbBuffer, size_t *pBytesReturned);

    /**
     * onboard_set_logging_callback() : Set a logging callback.
     *
     * @param pCallbackFn A ptr to a callback function of type \ref tQO_LOGGER_CALLBACK_FN.
     *                    See \ref tQO_LOGGER_CALLBACK_FN above for the details of this callback function.
     */
    extern void onboard_set_logging_callback(tQO_LOGGER_CALLBACK_FN *pCallbackFn);

    /**
     * Use Onboards logger to log message.
     *
     * @param loglevel  The severity of the log entry. The value is one of those in tONBOARD_LOGLEVEL
     * @param psMsg     A pointer to a null-terminated string containing the log message.
     * @param ...       Optional parameters to be passed into the string formatter.
     */
    extern void onboard_log_message(const tONBOARD_LOGLEVEL logLevel, const char *pMsg, ...);

    /**
     * Clear the logging callback
     */
    extern void onboard_clear_logging_callback(void);

    /**
     * onboard_get_error_description() : Obtain the most recent error message generated by Quantum Origin.
     *
     * @return A pointer to a null-terminated string containing the latest error message. The caller should *not* attempt to free this string.
     */
    extern const char *onboard_get_error_description(void);

    /**
     * onboard_get_error_code() : Obtain the most recent error code generated by Quantum Origin.
     *
     * @return The most recent error code
     */
    extern tQO_ONBOARD_RESULT onboard_get_error_code(void);

    /**
     * onboard_destroy() : Destroy the given Onboard context.
     *
     * @param pOnboard Pointer to a context previously obtained from onboard_init_from_config_file() or onboard_init_from_config_struct()
     */
    extern void onboard_destroy(tQO_ONBOARD_CTX *pOnboard);

#ifdef __cplusplus
}
#endif
