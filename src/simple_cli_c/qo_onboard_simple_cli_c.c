#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qo_hexdump.h"
#include "qo_onboard/qo_onboard_c.h"

#define BUFFER_SIZE 400U

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
// Main
//--------------------------------------------------------------
int main(int argc, char **argv)
{
    bool verboseMode          = false;
    tQO_ONBOARD_CTX *pOnboard = NULL;
    char *szConfigFilename    = NULL;
    uint32_t bytesRequested   = 0xFFFFFFFF;
    tQO_ONBOARD_RESULT onboardResult;
    unsigned char resultBuffer[BUFFER_SIZE];
    size_t bytesSupplied;
    int ret = 0;

    ////////////////////////////////////
    // Parse parameters
    ////////////////////////////////////
    for (int jj = 1; jj < argc; jj++)
    {
        if (strcmp(argv[jj], "--help") == 0)
        {
            printf("USAGE: qo_onboard_simple_cli_c [--verbose|-v] <ConfigFilename> [<BytesOfRandomness>] // (Default = 32 bytes)\n");
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[jj], "--verbose") == 0 || strcmp(argv[jj], "-v") == 0)
        {
            verboseMode = true;
            continue;
        }
        if (szConfigFilename == NULL)
        {
            szConfigFilename = argv[jj];
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

    if (verboseMode)
    {
        printf("INFO: ConfigFilename = %s\n", szConfigFilename ? szConfigFilename : "(Not specified)");
        printf("INFO: BytesRequested = %u\n", bytesRequested);
    }

    ////////////////////////////////////
    // Initialise Onboard
    ////////////////////////////////////
    onboard_set_logging_callback(my_qo_onboard_logger_callback);

    if (szConfigFilename == NULL)
    {
        ret = EXIT_FAILURE;
        fprintf(stderr, "ERROR: Config filename is required\n");
        goto error1;
    }

    if (verboseMode) printf("INFO: Calling onboard_init_from_config_file(\"%s\")\n", szConfigFilename);
    pOnboard = onboard_init_from_config_file(szConfigFilename, "");
    if (pOnboard == NULL)
    {
        ret = onboard_get_error_code();
        const char *szOnboardError = onboard_get_error_description();
        fprintf(stderr, "ERROR: Failed to initialize Onboard with onboard_init_from_config_file(\"%s\") (OnboardErrorMsg=%s)\n", szConfigFilename, szOnboardError?szOnboardError:"Unspecified");
        goto error1;
    }

    ////////////////////////////////////
    // Get Randomness
    ////////////////////////////////////
    if (verboseMode) printf("INFO: Calling onboard_get_randomness(%u)\n", bytesRequested);

    bytesSupplied = 0;
    onboardResult = onboard_get_randomness(pOnboard, resultBuffer, bytesRequested, &bytesSupplied);
    if (onboardResult != 0)
    {
        ret = onboard_get_error_code();
        const char *szOnboardError = onboard_get_error_description();
        fprintf(stderr, "ERROR: onboard_get_randomness failed with onboardResult=%d (OnboardErrorMsg=%s)\n", onboardResult, szOnboardError?szOnboardError:"Unspecified");
        onboard_destroy(pOnboard);
        goto error1;
    }

    ////////////////////////////////////
    // Print results
    ////////////////////////////////////
    if (verboseMode)
    {
        printf("INFO: onboard_get_randomness returned %lu bytes of randomness\n", (unsigned long)bytesSupplied);
        HexDump("INFO: onboard_get_randomness", resultBuffer, bytesSupplied);
    }
    else
    {
        for (size_t ii = 0; ii < bytesSupplied; ii++)
        {
            putc(resultBuffer[ii], stdout);
        }
    }

    if (verboseMode) fprintf(stdout, "INFO: Done.\n");
    ret = ERC_OK;
error1:
    onboard_destroy(pOnboard);
    onboard_clear_logging_callback();

    if (ret)
    {
        fprintf(stderr, "ERROR: qo_onboard_simple_cli_c failed with errCode=%d\n", ret);
    }
    return ret;
}
