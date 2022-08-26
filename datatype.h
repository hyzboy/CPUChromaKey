#pragma once

#include<stdint.h>

using int8=int8_t;
using int16=int16_t;
using int32=int32_t;
using int64=int64_t;

using uint8=uint8_t;
using uint16=uint16_t;
using uint32=uint32_t;
using uint64=uint64_t;

using uint=unsigned int;

struct pos2i
{
    int16 x,y;
};

union color3f
{
    struct{float r,g,b;};
    struct{float y,cb,cr;};
};

union color4f
{
    struct{float r,g,b,a;};
    struct{float y,cb,cr,a;};
};

union color4b
{
    uint32 rgba;
    struct{uint8 r,g,b,a;};
    struct{uint8 y,cb,cr,a;};
};