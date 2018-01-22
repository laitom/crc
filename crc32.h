#ifndef CRC32_H
#define CRC32_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t reflect8(uint8_t x) {
    return ((x*0x0802LU & 0x22110LU) | (x*0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

static uint32_t reflect32(uint32_t x) {
    return (reflect8(x & 0xFF) << 24) | (reflect8((x & 0xFF00) >> 8) << 16)
	| (reflect8((x & 0xFF0000) >> 16) << 8) | (reflect8((x & 0xFF000000) >> 24));
}

static uint32_t __crc32(uint8_t *msg, size_t len, uint32_t poly, uint32_t init, bool refin, bool refout, uint32_t xorout) {
    uint32_t ret = init;

#ifndef CRC32_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= refin ? (reflect8(*(msg+i)) << 24) : (*(msg+i) << 24);
	for (int j = 0; j < 8; ++j)
	    ret = ((ret & 0x80000000) != 0) ? ((ret << 1) ^ poly) : ret << 1;
    }
#else
    uint32_t table[256] = { 0 };
    uint32_t temp;

    if (table[1] == 0) {
	for (int i = 0; i < 256; ++i) {
	    temp = (uint32_t) (i << 24);
	    
	    for (int j = 0; i < 8; ++j)
		temp = ((temp & 0x80000000) != 0) ? ((temp << 1) ^ poly) : temp << 1;
	    
	    table[i] = temp;
	}
    }

    for (int i = 0; i < len; ++i)
	ret = (temp << 8) ^ (table[(ret ^ (*(msg+i) << 24)) >> 24]);
#endif // CRC32_USE_LOOKUP_TABLE

    ret = refout ? reflect32(ret) : ret;
    return ret ^ xorout;
}

#endif // CRC32_H
