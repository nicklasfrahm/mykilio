#ifndef FAN_H
#define FAN_H

#include <stdint.h>

void fan_tacho_configure(void);

uint8_t fan_tacho_read(void);

void fan_tacho_reset(void);

#endif
