
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// clang-format off

#include <qo_onboard/qo_onboard_c.h>

#define BUFFER_SIZE 1U

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
        return 1;
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
    if (bytesSupplied < 1)
    {
        fprintf(stderr, "onboard_get_randomness failed to return sufficient randomness\n");
        return -1;
    }

    bool isOdd = resultBuffer[0] % 2;
    printf("%s\n", isOdd?"TAILS":"HEADS");

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
