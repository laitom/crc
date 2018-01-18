/*
 * Algorithm parameters taken from http://reveng.sourceforge.net/crc-catalogue/all.htm
 */

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

static uint16_t __crc16(uint8_t *msg, size_t len, uint16_t poly, uint16_t init, bool refin, bool refout, uint16_t xorout) {
    uint16_t ret = init;

#ifndef CRC16_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	//ret ^= *(msg+i) << 8;
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

/*
 * CRC-16/AUG-CCITT (CRC-16/SPI-FUJITSU)
 */
uint16_t crc16_augccitt(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0x1D0F, false, false, 0x0000);
}

/*
 * CRC-16/BUYPASS (CRC-16/VERIFONE, CRC-16/UMTS)
 */
uint16_t crc16_buypass(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/CCITT-FALSE
 */
uint16_t crc16_ccittfalse(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xFFFF, false, false, 0x0000);
}

/*
 * CRC-16/CDMA2000
 */
uint16_t crc16_cdma2000(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0xC867, 0xFFFF, false, false, 0x0000);
}

/*
 * CRC-16/CMS
 */
uint16_t crc16_cms(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0xFFFF, false, false, 0x0000);
}

/*
 * CRC-16/DDS-110
 */
uint16_t crc16_dds110(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0x800d, false, false, 0x0000);
}

/*
 * CRC-16/DECT-R (R-CRC-16)
 */
uint16_t crc16_dectr(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x0589, 0x0000, false, false, 0x0001);
}

/*
 * CRC-16/DECT-X (X-CRC-16)
 */
uint16_t crc16_dectx(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x0589, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/DNP
 */
uint16_t crc16_dnp(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x3D65, 0x0000, true, true, 0xFFFF);
}

/*
 * CRC-16/EN-13757
 */
uint16_t crc16_en13757(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x3D65, 0x0000, false, false, 0xFFFF);
}

/*
 * CRC-16/GENIBUS (CRC-16/EPC, CRC-16/I-CODE, CRC-16/DARC)
 */
uint16_t crc16_genibus(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xFFFF, false, false, 0xFFFF);
}

/*
 * CRC-16/GSM
 */
uint16_t crc16_gsm(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0x0000, false, false, 0xFFFF);
}

/*
 * CRC-16/LJ1200
 */
uint16_t crc16_lj1200(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x6F63, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/MAXIM
 */
uint16_t crc16_maxim(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0x0000, true, true, 0xFFFF);
}

/*
 * CRC-16/MCRF4XX
 */
uint16_t crc16_mcrf4xx(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xFFFF, true, true, 0x0000);
}

/*
 * CRC-16/OPENSAFETY-A
 */
uint16_t crc16_opensafetya(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x5935, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/OPENSAFETY-B
 */
uint16_t crc16_opensafetyb(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x755B, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/PROFIBUS (CRC-16/IEC-61158-2)
 */
uint16_t crc16_profibus(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1DCF, 0xFFFF, false, false, 0xFFFF);
}

/*
 * CRC-16/RIELLO
 */
uint16_t crc16_riello(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xB2AA, true, true, 0x0000);
}

/*
 * CRC-16/T10-DIF
 */
uint16_t crc16_t10dif(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8BB7, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/TELEDISK
 */
uint16_t crc16_teledisk(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0xA097, 0x0000, false, false, 0x0000);
}

/*
 * CRC-16/TMS37157
 */
uint16_t crc16_tms37157(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0x89EC, true, true, 0x0000);
}

/*
 * CRC-16/USB
 */
uint16_t crc16_usb(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0xFFFF, true, true, 0xFFFF);
}

/*
 * CRC-A
 */
uint16_t crca(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xC6C6, true, true, 0x0000);
}

/*
 * KERMIT (CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-CCITT)
 */
uint16_t kermit(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0x0000, true, true, 0x0000);
}

/*
 * MODBUS
 */
uint16_t modbus(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x8005, 0xFFFF, true, true, 0x0000);
}

/*
 * X-25 (CRC-16/IBM-SDLC, CRC-16/ISO-HDLC, CRC-B)
 */
uint16_t x25(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0xFFFF, true, true, 0xFFFF);
}

/*
 * XMODEM (ZMODEM, CRC-16/ACORN, CRC-16/LTE)
 */
uint16_t xmodem(uint8_t *msg, size_t len) {
    return __crc16(msg, len, 0x1021, 0x0000, false, false, 0x0000);
}

#endif // CRC16_H
