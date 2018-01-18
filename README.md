# crc

Usage:
```
#include <assert.h>
#include "crc8.h"

char *msg = "123456789";
uint8_t crc = crc8((uint8_t *) msg, 9, 0x07, 0x00, false, false, 0x00);
assert(crc == 0xF4);
```