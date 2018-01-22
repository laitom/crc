# crc

Usage:
```
#include <assert.h>
#include "crc8.h"

char *msg = "123456789";
// poly=0x07, init=0x00, refin=false, refout=false, xorout=0x00
uint8_t crc = crc8((uint8_t *) msg, 9, 0x07, 0x00, false, false, 0x00);
assert(crc == 0xF4);
```