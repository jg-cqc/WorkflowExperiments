
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// clang-format off

#include <qo_onboard/qo_onboard_c.h>

#define BUFFER_SIZE 32U
#define MINIMUM(a,b) (((a)<(b))?(a):(b))

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
    char *szConfigFilename    = NULL;
    char *szConfigNodePath    = NULL;
    tQO_ONBOARD_CTX *pOnboard = NULL;
    uint32_t bytesRequested;
    unsigned char resultBuffer[BUFFER_SIZE];
    tQO_ONBOARD_RESULT onboardResult;
    size_t bytesSupplied;
    int ret = 0;

    // Parse parameters
    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s <ConfigFilename> [<ConfigNodePath>]\n", argv[0]);
        return EXIT_FAILURE;
    }
    szConfigFilename = argv[1];
    if (argc == 3)
    {
        szConfigNodePath = argv[2];
    }

    if (szConfigFilename == NULL)
    {
        ret = EXIT_FAILURE;
        fprintf(stderr, "ERROR: Config filename is required\n");
        goto error1;
    }

    if (szConfigNodePath == NULL)
    {
        szConfigNodePath = ""; // Avoid error [13802] "Null pointer received for nodePath parameter"
    }

    // Initialise Onboard
    printf("INFO: Calling onboard_init_from_config_file(\"%s\", \"%s\")\n", szConfigFilename, szConfigNodePath);
    pOnboard = onboard_init_from_config_file(szConfigFilename, szConfigNodePath);
    if (pOnboard == NULL)
    {
        ret = onboard_get_error_code();
        const char *szOnboardError = onboard_get_error_description();
        fprintf(stderr, "ERROR: Failed to initialize Onboard with onboard_init_from_config_file(\"%s\") (OnboardErrorMsg=%s)\n", szConfigFilename, szOnboardError?szOnboardError:"Unspecified");
        goto error1;
    }

    // Get Randomness
    bytesRequested = sizeof(resultBuffer);
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
    printf("INFO: Number of bytes of randomness returned = %zu (should be %zu)\n", bytesSupplied, sizeof(resultBuffer));
    hex_dump("INFO: First 32 bytes of randomness", resultBuffer, MINIMUM(bytesSupplied,32));

    ret = ERC_OK;

error1:
    // Cleanup
    if (pOnboard)
    {
        onboard_destroy(pOnboard);
        pOnboard = NULL;
    }
    return ret;
}
