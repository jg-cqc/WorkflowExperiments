
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

// clang-format off

#include "qo_onboard/qo_onboard_c.h"


#define BUFFER_SIZE 400U

#define ONBOARD_CONFIG_SEED_SIGNATURE_MAXLEN (512) // For example, quantum_seed_signature_base64.txt    176 bytes
#define ONBOARD_CONFIG_SEED_CONTENT_MAXLEN (20480) // For example, quantum_seed_content_base64.txt      10888 bytes
char szSeedSignatureEval[ONBOARD_CONFIG_SEED_SIGNATURE_MAXLEN] = {"evaluation"};
char szSeedContentEval[ONBOARD_CONFIG_SEED_CONTENT_MAXLEN] = {"evaluation"};

#include "qo_onboard_sample_code_seed.h"

// Forward declaration to ensure that the interface function is correctly defined.
static tQO_LOGGER_CALLBACK_FN my_qo_onboard_logger_callback;

//--------------------------------------------------------------
// Logger callback
//--------------------------------------------------------------
static void my_qo_onboard_logger_callback(const tONBOARD_LOGLEVEL logLevel, const char *szMsg, size_t msgLen)
{
    // Store or present the inbound message somewhere.
    switch(logLevel)
    {
        case ONBOARD_LOGLEVEL_NONE     : break;
        case ONBOARD_LOGLEVEL_CRITICAL : fprintf(stderr, "[qo_onboard] critical: %.*s\n", (int)msgLen, szMsg); break;
        case ONBOARD_LOGLEVEL_ERROR    : fprintf(stderr, "[qo_onboard] error: %.*s\n", (int)msgLen, szMsg); break;
        case ONBOARD_LOGLEVEL_WARNING  : fprintf(stderr, "[qo_onboard] warn: %.*s\n", (int)msgLen, szMsg); break;
        case ONBOARD_LOGLEVEL_INFO     : fprintf(stderr, "[qo_onboard] info: %.*s\n", (int)msgLen, szMsg); break;
        case ONBOARD_LOGLEVEL_DEBUG    : fprintf(stderr, "[qo_onboard] debug: %.*s\n", (int)msgLen, szMsg); break;
        default:
        case ONBOARD_LOGLEVEL_TRACE    : fprintf(stderr, "[qo_onboard] trace: %.*s\n", (int)msgLen, szMsg); break;
    }
}

//--------------------------------------------------------------
// Simple hex dump
//--------------------------------------------------------------
static void hex_dump(const char *szTitle, const uint8_t *pData, size_t cbData)
{
    printf("%s [%lu bytes]: ", szTitle ? szTitle : "HEXDUMP: ", (unsigned long)cbData);
    if (pData && cbData)
    {
        for (int ii = 0; ii < cbData; ii++)
        {
            printf("%2.2X ", pData[ii]);
        }
    }
    printf("\n");
}

//--------------------------------------------------------------
// Main
//--------------------------------------------------------------
int main(int argc, char **argv)
{
    bool verboseMode          = false;
    tONBOARD_CONFIG *pOnboardConfig = NULL;
    tQO_ONBOARD_CTX *pOnboard = NULL;
    uint32_t bytesRequested   = 0xFFFFFFFF;
    unsigned char resultBuffer[BUFFER_SIZE];
    tQO_ONBOARD_RESULT onboardResult;
    size_t bytesSupplied;
    int ret = 0;

    // Parse parameters
    for (int jj = 1; jj < argc; jj++)
    {
        if (strcmp(argv[jj], "--help") == 0)
        {
            printf("USAGE: %s [--verbose|-v] [<BytesOfRandomness>] // (Default = 32 bytes)\n", argv[0]);
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[jj], "--verbose") == 0 || strcmp(argv[jj], "-v") == 0)
        {
            verboseMode = true;
            continue;
        }
        if (bytesRequested == 0xFFFFFFFF)
        {
            if (argv[jj][0] == '-')
            {
                fprintf(stderr, "ERROR: Number of bytes requested cannot be negative\n");
                return EXIT_FAILURE;
            }
            bytesRequested = (uint32_t)atol(argv[jj]);
            if (bytesRequested > BUFFER_SIZE)
            {
                fprintf(stderr, "ERROR: Number of bytes requested must be <= %u\n", BUFFER_SIZE);
                return EXIT_FAILURE;
            }
            continue;
        }
    }

    if (bytesRequested == 0xFFFFFFFF)
    {
        bytesRequested = 32;
    }

    // Print some progress information
    if (verboseMode)
    {
        printf("INFO: BytesRequested = %u\n", bytesRequested);
    }

    // Initialise Onboard
    onboard_set_logging_callback(my_qo_onboard_logger_callback);

    pOnboardConfig = onboard_setopt_init();
    if (!pOnboardConfig) { ret = ERC_UNSPECIFIED_ERROR; goto error1; }
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_LOGGING_MODE    , ONBOARD_LOGMODE_STDERR    ); if (ret) { goto error1; }
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_LOGGING_LEVEL   , ONBOARD_LOGLEVEL_ERR      ); if (ret) { goto error1; }
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_CACHE_TYPE      , ONBOARD_CACHETYPE_CACHING ); if (ret) { goto error1; } // e.g. [caching] or [none]
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_CACHE_SIZE      , 10240                     ); if (ret) { goto error1; } // e.g. [1048576]
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_CACHE_PREFILL   , 10240                     ); if (ret) { goto error1; } // e.g. [128]
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_CACHE_REFILL_AT , 2048                      ); if (ret) { goto error1; } // e.g. [1024]
    ret = onboard_setopt_int  (pOnboardConfig, ONBOARD_OPT_WSR_TYPE        , ONBOARD_WSRTYPE_RDSEED    ); if (ret) { goto error1; } // e.g. [FILE]

    if (verboseMode)
    {
        printf("INFO: Size of Seed Signature = %zu\n", sizeof(seedSignature)); // e.g. 132
        printf("INFO: Size of Seed Content = %zu\n", sizeof(seedContent  )); // e.g. 8164
    }
    ret = onboard_setopt_bytes(pOnboardConfig, ONBOARD_OPT_SEED_SIGNATURE  , (uint8_t *)seedSignature, sizeof(seedSignature) ); if (ret) { goto error1; }
    ret = onboard_setopt_bytes(pOnboardConfig, ONBOARD_OPT_SEED_CONTENT    , (uint8_t *)seedContent  , sizeof(seedContent  ) ); if (ret) { goto error1; }

    if (verboseMode) printf("INFO: Calling onboard_init_from_config_struct()\n");

    pOnboard = onboard_init_from_config_struct(pOnboardConfig);
    if (pOnboard == NULL)
    {
        ret = onboard_get_error_code();
        const char *szOnboardError = onboard_get_error_description();
        fprintf(stderr, "ERROR: Failed to initialize Onboard with onboard_init_from_config_struct() (OnboardErrorMsg=%s)\n", szOnboardError?szOnboardError:"Unspecified");
        goto error1;
    }
    // The pOnboardConfig structure is no longer needed, and can be destroyed.
    onboard_setopt_cleanup(pOnboardConfig);
    pOnboardConfig = NULL;

    // Get Randomness
    if (verboseMode) printf("INFO: Calling onboard_get_randomness(%u)\n", bytesRequested);

    bytesSupplied = 0;
    onboardResult = onboard_get_randomness(pOnboard, resultBuffer, bytesRequested, &bytesSupplied);
    if (onboardResult != 0)
    {
        ret = onboard_get_error_code();
        const char *szOnboardError = onboard_get_error_description();
        fprintf(stderr, "ERROR: onboard_get_randomness failed with onboardResult=%d (OnboardErrorMsg=%s)\n", onboardResult, szOnboardError?szOnboardError:"Unspecified");
        goto error1;
    }

    // Print results
    if (verboseMode)
    {
        printf("INFO: onboard_get_randomness returned %zu bytes of randomness\n", bytesSupplied);
        hex_dump("INFO: onboard_get_randomness", resultBuffer, bytesSupplied);
    }
    else
    {
        for (size_t ii = 0; ii < bytesSupplied; ii++)
        {
            putc(resultBuffer[ii], stdout);
        }
    }

    if (verboseMode) fprintf(stdout, "INFO: %s Done.\n", argv[0]);
    ret = ERC_OK;

error1:
    // Cleanup
    if (pOnboardConfig)
    {
        onboard_setopt_cleanup(pOnboardConfig);
        pOnboardConfig = NULL;
    }
    if (pOnboard)
    {
        onboard_destroy(pOnboard);
        pOnboard = NULL;
    }
    onboard_clear_logging_callback();

    if (ret)
    {
        fprintf(stderr, "ERROR: %s failed with errCode=%d\n", argv[0], ret);
    }
    return ret;
}
