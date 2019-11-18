#pragma once

#include <stdint.h>

// TODO other location maybe?
#define IR_DIST_MM          9
#define MID_IR_POS_MM     139.5

#define MAXLINES      (3u)

typedef struct
{
    int16_t lines[MAXLINES];
    uint8_t cnt;
    uint8_t cross;
}
Line;
