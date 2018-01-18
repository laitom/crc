#ifndef CRC16_H
#define CRC16_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t reflect8(uint8_t x) {
    return ((x*0x0802LU & 0x22110LU) | (x*0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

static uint16_t reflect16(uint16_t x) {
    return (reflect8(x & 0xFF) << 8) | reflect8((x & 0xFF00) >> 8);
}

static uint16_t crc16(uint8_t *msg, size_t len, uint16_t poly, uint16_t init, bool refin, bool refout, uint16_t xorout) {
    uint16_t ret = init;

#ifndef CRC16_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= refin ? (reflect8(*(msg+i)) << 8) : (*(msg+i) << 8);
	for (int j = 0; j < 8; ++j)
	    ret = ((ret & 0x8000) != 0) ? ((ret << 1) ^ poly) : ret << 1;
    }
#else
    static uint16_t table[256] = { 0 };
    uint16_t temp;

    if (table[1] == 0) {
	for (int i = 0; i < 256; ++i) {
	    temp = (uint16_t) (i << 8);
	    
	    for (int j = 0; j < 8; ++j)
		temp = ((temp & 0x8000) != 0) ? ((temp << 1) ^ poly) : temp << 1;
	    
	    table[i] = temp;
	}
    }

    for (int i = 0; i < len; ++i)
	ret = (ret << 8) ^ table[(ret >> 8) ^ *(msg+i)];
#endif // CRC16_USE_LOOKUP_TABLE

    ret = refout ? reflect16(ret) : ret;
    return ret ^ xorout;
}

#endif // CRC16_H
