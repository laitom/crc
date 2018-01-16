#ifndef CRC16_H
#define CRC16_H

#include <stddef.h>
#include <stdint.h>

/*
 * Algorithm parameters taken from http://reveng.sourceforge.net/crc-catalogue/all.htm
 */
#define CRC16_AUGCCITT_POLY     0x1021
#define CRC16_AUGCCITT_INIT     0x1D0F
#define CRC16_AUGCCITT_XOROUT   0x0000
#define CRC16_BUYPASS_POLY      0x8005
#define CRC16_BUYPASS_INIT      0x0000
#define CRC16_BUYPASS_XOROUT    0x0000
#define CRC16_CCITTFALSE_POLY   0x1021
#define CRC16_CCITTFALSE_INIT   0xFFFF
#define CRC16_CCITTFALSE_XOROUT 0x0000
#define CRC16_CDMA2000_POLY     0xC867
#define CRC16_CDMA2000_INIT     0xFFFF
#define CRC16_CDMA2000_XOROUT   0x0000
#define CRC16_CMS_POLY          0x8005
#define CRC16_CMS_INIT          0xFFFF
#define CRC16_CMS_XOROUT        0x0000
#define CRC16_DDS110_POLY       0x8005
#define CRC16_DDS110_INIT       0x800D
#define CRC16_DDS110_XOROUT     0x0000
#define CRC16_DECTR_POLY        0x0589
#define CRC16_DECTR_INIT        0x0000
#define CRC16_DECTR_XOROUT      0x0001
#define CRC16_DECTX_POLY        0x0589
#define CRC16_DECTX_INIT        0x0000
#define CRC16_DECTX_XOROUT      0x0000
#define CRC16_GENIBUS_POLY      0x1021
#define CRC16_GENIBUS_INIT      0xFFFF
#define CRC16_GENIBUS_XOROUT    0xFFFF
#define CRC16_GSM_POLY          0x1021
#define CRC16_GSM_INIT          0x0000
#define CRC16_GSM_XOROUT        0xFFFF

static uint16_t __crc16(uint8_t *msg, size_t len, uint16_t poly, uint16_t init, uint16_t xorout) {
    uint16_t ret = init;

#ifndef CRC16_USE_LOOKUP_TABLE
    for (int i = 0; i < len; ++i) {
	ret ^= *(msg+i) << 8;
	for (int j = 0; j < 8; ++j)
	    ret = ((ret & 0x8000) != 0) ? ((ret << 1) ^ poly) : ret << 1;
    }
#else
    uint16_t table[256] = { 0 };
    uint16_t temp;

    if (table[1] == 0) {
	for (int i = 0; i < 256; ++i) {
	    temp = (uint16_t) (i << 8);
	    
	    for (int j = 0; j < 8; ++j)
		temp = ((temp & 0x8000) != 0) ? ((temp << 1) ^ poly) : temp << 1;
	    
	    table[i] = temp;
	}
    }

    for (int i = 0; i < len; ++i) {
	ret = (ret << 8) ^ table[(ret >> 8) ^ *(msg+i)];
    }
#endif

    return ret ^ xorout;
}

/*
 * CRC-16/AUG-CCITT (CRC-16/SPI-FUJITSU)
 */
uint16_t crc16_augccitt(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_AUGCCITT_POLY, CRC16_AUGCCITT_INIT, CRC16_AUGCCITT_XOROUT);
}

/*
 * CRC-16/BUYPASS (CRC-16/VERIFONE, CRC-16/UMTS)
 */
uint16_t crc16_buypass(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_BUYPASS_POLY, CRC16_BUYPASS_INIT, CRC16_BUYPASS_XOROUT);
}

/*
 * CRC-16/CCITT-FALSE
 */
uint16_t crc16_ccittfalse(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_CCITTFALSE_POLY, CRC16_CCITTFALSE_INIT, CRC16_CCITTFALSE_XOROUT);
}

/*
 * CRC-16/CDMA2000
 */
uint16_t crc16_cdma2000(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_CDMA2000_POLY, CRC16_CDMA2000_INIT, CRC16_CDMA2000_XOROUT);
}

/*
 * CRC-16/CMS
 */
uint16_t crc16_cms(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_CMS_POLY, CRC16_CMS_INIT, CRC16_CMS_XOROUT);
}

/*
 * CRC-16/DDS-110
 */
uint16_t crc16_dds110(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_DDS110_POLY, CRC16_DDS110_INIT, CRC16_DDS110_XOROUT);
}

/*
 * CRC-16/DECT-R (R-CRC-16)
 */
uint16_t crc16_dectr(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_DECTR_POLY, CRC16_DECTR_INIT, CRC16_DECTR_XOROUT);
}

/*
 * CRC-16/DECT-X (X-CRC-16)
 */
uint16_t crc16_dectx(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_DECTX_POLY, CRC16_DECTX_INIT, CRC16_DECTX_XOROUT);
}

/*
 * CRC-16/GENIBUS (CRC-16/EPC, CRC-16/I-CODE, CRC-16/DARC)
 */
uint16_t crc16_genibus(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_GENIBUS_POLY, CRC16_GENIBUS_INIT, CRC16_GENIBUS_XOROUT);
}

/*
 * CRC-16/GSM
 */
uint16_t crc16_gsm(uint8_t *msg, size_t len) {
    return __crc16(msg, len, CRC16_GSM_POLY, CRC16_GSM_INIT, CRC16_GSM_XOROUT);
}

#endif // CRC16_H
