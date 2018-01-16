#include <stddef.h>
#include <stdint.h>

#define CRC8_POLY            0x07
#define CRC8_INIT            0x00
#define CRC8_XOROUT          0x00

#define CRC8_CDMA2000_POLY   0x9B
#define CRC8_CDMA2000_INIT   0xFF
#define CRC8_CDMA2000_XOROUT 0x00

#define CRC8_DVBS2_POLY      0xD5
#define CRC8_DVBS2_INIT      0x00
#define CRC8_DVBS2_XOROUT    0x00

#define CRC8_ITU_POLY        0x07
#define CRC8_ITU_INIT        0x00
#define CRC8_ITU_XOROUT      0x55

#define CRC8_LTE_POLY        0x9B
#define CRC8_LTE_INIT        0x00
#define CRC8_LTE_XOROUT      0x00

static uint8_t reverse8(uint8_t x) {
    return ((x*0x0802LU & 0x22110LU) | (x*0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

static uint8_t __crc8(uint8_t *msg, size_t len, uint8_t poly, uint8_t init, uint8_t xorout) {
    uint8_t ret = init;

#ifndef CRC8_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= *(msg+i);
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

    return ret ^ xorout;
}

/*
 * CRC-8
 */
uint8_t crc8(uint8_t *msg, size_t len) {
    return __crc8(msg, len, CRC8_POLY, CRC8_INIT, CRC8_XOROUT);
}

/*
 * CRC-8/CDMA2000
 */
uint8_t crc8_cdma2000(uint8_t *msg, size_t len) {
    return __crc8(msg, len, CRC8_CDMA2000_POLY, CRC8_CDMA2000_INIT, CRC8_CDMA2000_XOROUT);
}

/*
 * CRC-8/DVB-S2
 */
uint8_t crc8_dvbs2(uint8_t *msg, size_t len) {
    return __crc8(msg, len, CRC8_DVBS2_POLY, CRC8_DVBS2_INIT, CRC8_DVBS2_XOROUT);
}

/*
 * CRC-8/ITU
 */
uint8_t crc8_itu(uint8_t *msg, size_t len) {
    return __crc8(msg, len, CRC8_ITU_POLY, CRC8_ITU_INIT, CRC8_ITU_XOROUT);
}

/*
 * CRC-8/LTE
 */
uint8_t crc8_lte(uint8_t *msg, size_t len) {
    return __crc8(msg, len, CRC8_LTE_POLY, CRC8_LTE_INIT, CRC8_LTE_XOROUT);
}
