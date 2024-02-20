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
static void my_qo_onboard_logger_callback(const tONBOARD_LOGLEVEL logLevel, tQO_ONBOARD_RESULT errorCode, const char *szMsg, size_t msgLen)
{
    // Store or present the inbound message somewhere.
    switch(logLevel)
    {
        case ONBOARD_LOGLEVEL_NONE     : break;
        case ONBOARD_LOGLEVEL_CRITICAL : fprintf(stderr, "[qo_onboard] critical: status=%d (%s)\n", errorCode, szMsg); break;
        case ONBOARD_LOGLEVEL_ERROR    : fprintf(stderr, "[qo_onboard] error: status=%d (%s)\n", errorCode, szMsg); break;
        case ONBOARD_LOGLEVEL_WARNING  : fprintf(stderr, "[qo_onboard] warn: status=%d (%s)\n", errorCode, szMsg); break;
        case ONBOARD_LOGLEVEL_INFO     : fprintf(stderr, "[qo_onboard] info: status=%d (%s)\n", errorCode, szMsg); break;
        case ONBOARD_LOGLEVEL_DEBUG    : fprintf(stderr, "[qo_onboard] debug: status=%d (%s)\n", errorCode, szMsg); break;
        default:
        case ONBOARD_LOGLEVEL_TRACE    : fprintf(stderr, "[qo_onboard] trace: status=%d (%s)\n", errorCode, szMsg); break;
    }
}

//--------------------------------------------------------------
// Main
//--------------------------------------------------------------
int main(int argc, char **argv)
{
    bool verboseMode          = false;
    tQO_ONBOARD_CTX *pOnboard = NULL;
    char *szConfigFilename    = "config-sample-01.yml";
    uint32_t bytesRequested   = 0xFFFFFFFF;
    tQO_ONBOARD_RESULT onboardResult;
    unsigned char resultBuffer[BUFFER_SIZE];
    size_t bytesSupplied;

    bytesRequested = 32;

    ////////////////////////////////////
    // Initialise Onboard
    ////////////////////////////////////
    onboard_set_logging_callback(my_qo_onboard_logger_callback);

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
    for (size_t ii = 0; ii < bytesSupplied; ii++)
    {
        putc(resultBuffer[ii], stdout);
    }

    ret = ERC_OK;

error1:
    onboard_destroy(pOnboard);
    onboard_clear_logging_callback();

    return ret;
}
