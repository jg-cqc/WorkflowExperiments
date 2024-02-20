#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void HexDump(const char *szTitle, const uint8_t *pData, size_t cbData);

#ifdef __cplusplus
}
#endif
