#pragma once

#include"datatype.h"

/**
 * 纯引用型位图数据，数据由用户外部传入，需用户自行释放内存
 */
template<typename T> struct BitmapData
{
    int width,height;

    T *data;

public:

    BitmapData()
    {
        data=nullptr;
    }

    BitmapData(T *ptr,int w,int h)
    {
        data=ptr;
        width=w;
        height=h;
    }

    virtual ~BitmapData()=default;
};

/**
 * 自分配位图数据，自动分配内存，自动释放
 */
template<typename T> struct BitmapDataAlloc:public BitmapData<T>
{
public:

    using BitmapData<T>::BitmapData;

    BitmapDataAlloc(int w,int h)
    {
        this->width=w;
        this->height=h;
        this->data=new T[w*h];
    }

    ~BitmapDataAlloc()override
    {
        delete[] this->data;
    }
};

/**
 * 位图访问视图，需外部传入位图数据对象
 */
template<typename T> struct BitmapView
{
protected:
    
    BitmapData<T> *bmp;

public:

    BitmapView(BitmapData<T> *bd)
    {   
        bmp=bd;
    }

    ~BitmapView()
    {
        if(bmp)
            delete bmp;
    }

    const int width()const{return bmp?bmp->width:0;}
    const int height()const{return bmp?bmp->height:0;}

    T *data()
    {
        return bmp?bmp->data:nullptr;
    }

    bool get(const int x,const int y,T &color)
    {
        if(!bmp
         ||x<0||x>=bmp->width
         ||y<0||y>=bmp->height)return false;

        color=bmp->data[x+y*bmp->width];
        return(true);
    }

    void set(const pos2i &pos,const T &color)
    {
        if(!bmp
         ||pos.x<0||pos.x>=bmp->width
         ||pos.y<0||pos.y>=bmp->height)return;

         bmp->data[pos.x+pos.y*bmp->width]=color;
    }
};//template<typename T> struct BitmapView

using Bitmap3b=BitmapView<color3b>;
using Bitmap4b=BitmapView<color4b>;
using Bitmap4f=BitmapView<color4f>;

bool SaveToTGA(const char *filename,Bitmap4b *bmp);
Bitmap4b *LoadFromTGA(const char *filename);

bool RGB2YCbCr(Bitmap4f *,Bitmap4b *);
