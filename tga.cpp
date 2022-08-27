#include"bitmap2d.h"
#include<io.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>

#pragma pack(push,1)
struct TGAHeader
{
    uint8 id;
    uint8 color_map_type;
    uint8 image_type;               // 1 colormap image ,2 true-color,3 grayscale

    uint16 color_map_first;
    uint16 color_map_length;
    uint8 color_map_size;

    uint16 x_origin;
    uint16 y_origin;

    uint16 width;
    uint16 height;
    uint8 bit;
    uint8 image_desc;
};

union TGAImageDesc
{
    uint8 image_desc;
    
    struct
    {
        uint alpha_depth:4;
        uint right_to_left:1;       //1: right to left, 0:left to right
        uint top_to_bottom:1;       //1: top to bottom, 0:bottom to top
    };
};

constexpr uint TGAHeaderLength=sizeof(TGAHeader);
#pragma pack(pop)

bool SaveToTGA(const char *filename,Bitmap4b *bmp)
{
    int fp;
    errno_t result;
    
    result=_sopen_s(&fp,filename,_O_BINARY|_O_WRONLY|_O_CREAT|_O_TRUNC,_SH_DENYNO,S_IREAD|_S_IWRITE);

    if(result)
        return(false);

    if(fp==-1)return(false);

    TGAImageDesc desc;

    desc.alpha_depth=8;
    desc.right_to_left=0;
    desc.top_to_bottom=1;

    TGAHeader header;
    memset(&header,0,sizeof(TGAHeader));

    header.image_type=2;
    header.width=bmp->width();
    header.height=bmp->height();
    header.bit=32;
    header.image_desc=desc.image_desc;
    
    _write(fp,&header,TGAHeaderLength);
    _write(fp,bmp->data(),bmp->width()*bmp->height()*4);

    _close(fp);

    printf("Save Screen to %s\n",filename);

    return(true);
}

void RGB2RGBA(color4b *rgba,const color3b *rgb,uint count)
{
    for(uint i=0;i<count;i++)
    {
        *rgba=*rgb;
        rgba->a=255;

        ++rgba;
        ++rgb;
    }
}

Bitmap4b *LoadFromTGA(const char *filename)
{
    int fp;
    errno_t result;

    result=_sopen_s(&fp,filename,_O_BINARY|O_RDONLY,_SH_DENYNO,S_IREAD);

    if(fp==-1)
        return(nullptr);

    TGAHeader header;
    TGAImageDesc desc;

    _read(fp,&header,sizeof(TGAHeader));

    if(header.image_type!=2)
    {
        _close(fp);
        return(nullptr);
    }
    
    Bitmap4b *bmp=new Bitmap4b(new BitmapDataAlloc<color4b>(header.width,header.height));

    desc.image_desc=header.image_desc;

    if(header.bit==24)
    {
        if(desc.top_to_bottom)
        {
            color3b *rgb=new color3b[header.width*header.height];

            _read(fp,rgb,header.width*header.height*3);

            RGB2RGBA(bmp->data(),rgb,header.width*header.height);

            delete[] rgb;
        }
        else
        {
            color3b *rgb=new color3b[header.width];
            color4b *tp=bmp->data()+(header.height-1)*header.width;

            for(int r=0;r<header.height;r++)
            {
                _read(fp,rgb,header.width*3);
                RGB2RGBA(tp,rgb,header.width);
                tp-=header.width;
            }

            delete[] rgb;
        }
    }
    else if(header.bit==32)
    {
        if(desc.top_to_bottom)
        {
            _read(fp,bmp->data(),header.width*header.height*4);
        }
        else
        {
            color4b *tp=bmp->data()+(header.height-1)*header.width;

            for(int r=0;r<header.height;r++)
            {
                _read(fp,tp,header.width*4);
                tp-=header.width;
            }
        }
    }

    _close(fp);
    return(bmp);
}