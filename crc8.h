#ifndef CRC8_H
#define CRC8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t reflect8(uint8_t x) {
    return ((x*0x0802LU & 0x22110LU) | (x*0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

static uint8_t crc8(uint8_t *msg, size_t len, uint8_t poly, uint8_t init, bool refin, bool refout, uint8_t xorout) {
    uint8_t ret = init;

#ifndef CRC8_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= refin ? reflect8(*(msg+i)) : *(msg+i);
	for (int j = 0; j < 8; ++j)
	    ret = ((ret & 0x80) != 0) ? ((ret << 1) ^ poly) : ret << 1;
    }
#else
    static uint8_t table[256] = { 0 };
    uint8_t temp;

    if (table[1] == 0) {
	for (int i = 0; i < 256; ++i) {
	    temp = (uint8_t) i;
	    
	    for (int j = 0; j < 8; ++j)
		temp = ((temp & 0x80) != 0) ? ((temp << 1) ^ poly) : temp << 1;

	    table[i] = temp;
	}
    }

    for (int i = 0; i < len; ++i)
	ret = table[*(msg+i) ^ ret];
#endif // CRC8_USE_LOOKUP_TABLE

    ret = refout ? reflect8(ret) : ret;
    return ret ^ xorout;
}

#endif // CRC8_H
