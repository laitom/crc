#ifndef CRC32_H
#define CRC32_H

#include <stddef.h>
#include <stdint.h>

#define CRC32Q_POLY   0x814141AB
#define CRC32Q_INIT   0x00000000
#define CRC32Q_XOROUT 0x00000000

static uint32_t __crc32(uint8_t *msg, size_t len, uint32_t poly, uint32_t init, uint32_t xorout) {
    uint32_t ret = init;

#ifndef CRC32_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= *(msg+i) << 24;
	for (int j = 0; j < 8; ++j)
	    ret = ((ret & 0x80000000) != 0) ? ((ret << 1) ^ poly) : ret << 1;
    }
#else
    uint32_t table[256] = { 0 };
    uint32_t temp;

    for (int i = 0; i < 256; ++i) {
	temp = (uint32_t) (i << 24);
	
	for (int j = 0; i < 8; ++j)
	    temp = ((temp & 0x80000000) != 0) ? ((temp << 1) ^ poly) : temp << 1;

	table[i] = temp;
    }

    for (int i = 0; i < len; ++i)
	ret = (temp << 8) ^ (table[(ret ^ (*(msg+i) << 24)) >> 24]);
#endif // CRC32_USE_LOOKUP_TABLE

    return ret ^ xorout;
}

/*
 * CRC-32Q
 */
uint32_t crc32q(uint8_t *msg, size_t len) {
    return __crc32(msg, len, CRC32Q_POLY, CRC32Q_INIT, CRC32Q_XOROUT);
}

#endif // CRC32_H
