
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//--------------------------------------------------------------
// HexDump
//--------------------------------------------------------------
void HexDump(const char *szTitle, const uint8_t *pData, size_t cbData)
{
    bool insert_spaces_between_bytes = true;
    size_t bytes_per_line = 32;
    bool append_ascii_representation = true;
    int StartAddress;
    int CharsProcessed;
    unsigned int ii;
    char *pSourceBuffer = NULL;
    size_t cbOutputBuffer = 0;
    char *pOutputBuffer = NULL;

    printf("%s [%lu bytes]:\n", szTitle ? szTitle : "HEXDUMP: ", (unsigned long)cbData);

    // cppcheck-suppress constArgument
    pSourceBuffer = malloc(bytes_per_line + 1);
    if (pSourceBuffer == NULL)
    {
        fprintf(stderr, "ERROR: [hexdump] No memory for ascii buffer\n");
        return;
    }

    cbOutputBuffer = 10 +                                                    // Start Address: "%08X: "
                     (bytes_per_line*(insert_spaces_between_bytes?3:2)) +    // Data: "%02X " * N
                     (append_ascii_representation?(bytes_per_line+2):0) +    // ASCII: "%c" * N (in double quotes)
                     1 +                                                     // Terminating NULL
                     20;                                                     // For good measure
    pOutputBuffer = malloc(cbOutputBuffer);
    if (pOutputBuffer == NULL)
    {
        fprintf(stderr, "ERROR: [hexdump] No memory for output buffer\n");
        free(pSourceBuffer);
        return;
    }

    CharsProcessed = 0;
    StartAddress = 0;
    memset(pSourceBuffer, 0, bytes_per_line + 1);

    char *pOneByte = (char *)pData;

    for (ii = 0; ii < cbData; ii++)
    {
        char ch = *pOneByte;
        pOneByte++;

        pSourceBuffer[CharsProcessed  ] = ch;
        pSourceBuffer[CharsProcessed+1] = 0;

        CharsProcessed++;

        if (CharsProcessed == bytes_per_line) // i.e. we already have all but this last char
        {
            int jj;

            snprintf(pOutputBuffer, cbOutputBuffer, "%08X: ", StartAddress); // Print offset (len = 10 chars)
            size_t cbOutputIdx = strlen(pOutputBuffer);
            StartAddress += CharsProcessed;
            for (jj = 0; jj < bytes_per_line; jj++)
            {
                 // Print byte in hex (len = 2 chars)
                 // (Add '& 0xFF' for annoying Tc hex bug) [What is the "Tc hex bug?"]
                snprintf( pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, "%02X", pSourceBuffer[jj] & 0xFF);
                cbOutputIdx += 2;
                if (insert_spaces_between_bytes)
                {
                    snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, " "); // (len = 1 char)
                    cbOutputIdx += 1;
                }
                if (!isprint(pSourceBuffer[jj]))
                    pSourceBuffer[jj] = '.';
            }
            // Append ASCII chars
            if (append_ascii_representation)
            {
                snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, "\"%s\"", pSourceBuffer); // Print string in ascii (len = bytes_per_line + 2 chars)
            }
            // Output it to the console
            printf("%s\n", pOutputBuffer);

            pSourceBuffer[0] = 0;
            CharsProcessed = 0;
        }
    }

    if (CharsProcessed > 0) // Some still unreported
    {
        int jj;

        snprintf(pOutputBuffer, cbOutputBuffer, "%08X: ", StartAddress); // Print offset
        size_t cbOutputIdx = strlen(pOutputBuffer);

        for (jj = 0; jj < bytes_per_line; jj++)
        {
            if (jj < CharsProcessed)
            {
                snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, "%02X", pSourceBuffer[jj] & 0xFF); // Print byte in hex (len = 2 chars)
                cbOutputIdx += 2;
                if (insert_spaces_between_bytes)
                {
                    snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, " "); // (len = 1 char)
                    cbOutputIdx += 1;
                }
                if (!isprint(pSourceBuffer[jj]))
                    pSourceBuffer[jj] = '.';
            }
            else
            {
                snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, "  "); // Print filler (len = 2 chars)
                cbOutputIdx += 2;
                if (insert_spaces_between_bytes)
                {
                    snprintf(pOutputBuffer + strlen(pOutputBuffer), cbOutputBuffer - cbOutputIdx, " "); // (len = 1 char)
                    cbOutputIdx += 1;
                }
                pSourceBuffer[jj] = ' ';
            }
        }
        // Append ASCII chars
        if (append_ascii_representation)
        {
            snprintf(pOutputBuffer + cbOutputIdx, cbOutputBuffer - cbOutputIdx, "\"%s\"", pSourceBuffer); // Print string in ascii
        }
        // Output it to the console
        printf("%s\n", pOutputBuffer);
    }

    free(pOutputBuffer);
    free(pSourceBuffer);
}
