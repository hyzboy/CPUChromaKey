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

union color3b
{
    struct{uint8 r,g,b;};
    struct{uint8 y,cb,cr;};
};

union color4f
{
    struct{float r,g,b,a;};
    struct{float y,cb,cr,a;};

public:

    color4f()
    {
        r=g=b=a=0;
    }

    color4f(const float _r,const float _g,const float _b,const float _a)
    {
        r=_r;
        g=_g;
        b=_b;
        a=_a;
    }

    const color4f operator*(const double &m)const
    {
        return color4f(r*m,g*m,b*m,a*m);
    }

    const color4f operator-(const color4f &c)const
    {
        return color4f(r-c.r,g-c.g,b-c.b,a-c.a);
    }

    void operator+=(const float v)
    {
        r+=v;
        g+=v;
        b+=v;
        a+=v;
    }
};

union color4b
{
    uint32 rgba;
    struct{uint8 r,g,b,a;};
    struct{uint8 y,cb,cr,a;};

    void operator = (const color3b &rgb)
    {
        r=rgb.r;
        g=rgb.g;
        b=rgb.b;
    }

    void operator = (const color4f &rgba)
    {
        r=uint8(rgba.r*255.0);
        g=uint8(rgba.g*255.0);
        b=uint8(rgba.b*255.0);
        a=uint8(rgba.a*255.0);
    }
};


template<typename T>
inline float rgb2y(const T &c)
{
    return float(0.299*c.r + 0.587*c.g + 0.114*c.b);
}

inline void RGB2YCbCr(color4f &ycbcr,const color4b &rgb)
{
    float r=float(rgb.r)/255.0;
    float g=float(rgb.g)/255.0;
    float b=float(rgb.b)/255.0;

    ycbcr.y =(       0.299   *r + 0.587   *g + 0.114   *b);
    ycbcr.cb=(0.5 + -0.168736*r - 0.331264*g + 0.5     *b);
    ycbcr.cr=(0.5 +  0.5     *r - 0.418688*g - 0.081312*b);
}

template<typename T>
inline T smoothstep(T edge0,T edge1,T x)
{
    T t;
    t = (x - edge0) / (edge1 - edge0);
    if(t<0)t=0;
    if(t>1)t=1;
    return t * t * (3.0 - 2.0 * t);
}

template<typename T>
inline T mix(T x,T y,T a)
{
    return x*(1-a)+y*a;
}
