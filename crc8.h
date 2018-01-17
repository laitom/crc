/*
 * Algorithm parameters taken from http://reveng.sourceforge.net/crc-catalogue/all.htm
 */

#ifndef CRC8_H
#define CRC8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t reverse8(uint8_t x) {
    return ((x*0x0802LU & 0x22110LU) | (x*0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}

static uint8_t __crc8(uint8_t *msg, size_t len, uint8_t poly, uint8_t init, bool refin, bool refout, uint8_t xorout) {
    uint8_t ret = init;

#ifndef CRC8_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= refin ? reverse8(*(msg+i)) : *(msg+i);
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

    ret = refout ? reverse8(ret) : ret;
    return ret ^ xorout;
}

/*
 * CRC-8
 */
uint8_t crc8(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x07, 0x00, false, false, 0x00);
}

/*
 * CRC-8/AUTOSAR
 */
uint8_t crc8_autosar(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x2F, 0xFF, false, false, 0xFF);
}

/*
 * CRC-8/BLUETOOTH
 */
uint8_t crc8_bluetooth(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0xA7, 0x00, true, true, 0x00);
}

/*
 * CRC-8/CDMA2000
 */
uint8_t crc8_cdma2000(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x9B, 0xFF, false, false, 0x00);
}

/*
 * CRC-8/DARC
 */
uint8_t crc8_darc(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x39, 0x00, true, true, 0x00);
}

/*
 * CRC-8/DVB-S2
 */
uint8_t crc8_dvbs2(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0xD5, 0x00, false, false, 0x00);
}

/*
 * CRC-8/EBU
 */
uint8_t crc8_ebu(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x1D, 0xFF, true, true, 0x00);
}

/*
 * CRC-8/GSM-A
 */
uint8_t crc8_gsma(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x1D, 0x00, false, false, 0x00);
}

/*
 * CRC-8/GSM-B
 */
uint8_t crc8_gsmb(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x49, 0x00, false, false, 0xFF);
}

/*
 * CRC-8/I-CODE
 */
uint8_t crc8_icode(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x1D, 0xFD, false, false, 0x00);
}

/*
 * CRC-8/ITU
 */
uint8_t crc8_itu(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x07, 0x00, false, false, 0x55);
}

/*
 * CRC-8/LTE
 */
uint8_t crc8_lte(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x9B, 0x00, false, false, 0x00);
}

/*
 * CRC-8/MAXIM (DOW-CRC)
 */
uint8_t crc8_maxim(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x31, 0x00, true, true, 0x00);
}

/*
 * CRC-8/OPENSAFETY
 */
uint8_t crc8_opensafety(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x2F, 0x00, false, false, 0x00);
}

/*
 * CRC-8/ROHC
 */
uint8_t crc8_rohc(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x07, 0xFF, true, true, 0x00);
}

/*
 * CRC-8/SAE-J1850
 */
uint8_t crc8_saej1850(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x1D, 0xFF, false, false, 0xFF);
}

/*
 * CRC-8/WCDMA
 */
uint8_t crc8_wcdma(uint8_t *msg, size_t len) {
    return __crc8(msg, len, 0x9B, 0x00, true, true, 0x00);
}

#endif // CRC8_H
