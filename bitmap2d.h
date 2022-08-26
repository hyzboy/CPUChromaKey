#pragma once

#include"datatype.h"

template<typename T> struct bitmap2d
{
    int width,height;

    T *data;

public:

    bitmap2d(T *ptr,int w,int h)
    {
        data=ptr;
        width=w;
        height=h;
    }
};

using BitmapRGBA8=bitmap2d<color4b>;
using BitmapFloat=bitmap2d<color4f>;

