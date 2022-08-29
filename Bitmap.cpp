#include"bitmap2d.h"

bool RGB2YCbCr(Bitmap4f *yuv,Bitmap4b *rgb)
{
    if(!yuv||!rgb)return(false);

    if(yuv->width()!=rgb->width()
     ||yuv->height()!=rgb->height())
        return(false);

    color4f *tp=yuv->data();
    color4b *sp=rgb->data();

    const uint count=rgb->width()*rgb->height();

    for(uint i=0;i<count;i++)
    {
        RGB2YCbCr(*tp,*sp);

        ++tp;
        ++sp;
    }

    return(true);
}
