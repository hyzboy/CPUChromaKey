#include"Chroma.h"

ChromaBitmap::ChromaBitmap()
{
    yuv=nullptr;
    final_bmp=nullptr;

    Cutoff=0.2*0.2;
    ColorFeathering=0.33*0.33;
    MaskFeathering=1.0;
    Sharpening=0.5;
    Despill=1.0;
    DespillLuminanceAdd=0.2;
}

ChromaBitmap::~ChromaBitmap()
{
    delete yuv;
    delete final_bmp;
}

const color4f ChromaBitmap::tex2d(const int x,const int y)const
{
    color4b rgb;

    bmp->get(x,y,rgb);

    return color4f( float(rgb.r)/255.0f,
                    float(rgb.g)/255.0f,
                    float(rgb.b)/255.0f,
                    1.0);
}

double ChromaBitmap::masked(const int x,const int y)
{
    color4f color;

    yuv->get(x,y,color);

    double temp=(chroma_color.cb-color.cb)*(chroma_color.cb-color.cb)
                +(chroma_color.cr-color.cr)*(chroma_color.cr-color.cr);

    if(temp<Cutoff)return 0;
    if(temp<ColorFeathering)return (temp-Cutoff)/CutoffColor;
    return 1;
}

void ChromaBitmap::compute_pixel(color4b *out_color,const int x,const int y)
{
    color4f color=tex2d(x,y);

    double c=masked(x,y);
    double r=masked(x+1,y);
    double l=masked(x-1,y);
    double d=masked(x,y+1);
    double u=masked(x,y-1);
    double rd=masked(x+1,y+1);
    double dl=masked(x-1,y+1);
    double lu=masked(x-1,y-1);
    double ur=masked(x+1,y+1);

    double blurContribution=(c+r+l+d+u+(rd+dl+lu+ur)*0.707)*0.12774655;
    double smoothedMask=smoothstep<double>(Sharpening,1.0f,mix(c,blurContribution,MaskFeathering));

    color4f result=color*smoothedMask;

    float v=float((2.0f*result.b+result.r)/4.0f);
    if(result.g>v)result.g=mix<float>(result.g,v,Despill);

    color4f dif=color-result;
    float desaturateDif=rgb2y(dif);

    result+=mix<float>(0,desaturateDif,DespillLuminanceAdd);

    out_color->r=uint8(result.r*255);
    out_color->g=uint8(result.g*255);
    out_color->b=uint8(result.b*255);
    out_color->a=uint8(smoothedMask*255);
}

Bitmap4b *ChromaBitmap::Chroma(Bitmap4b *b)
{
    bmp=b;

    if(yuv)
    {
        if(yuv->width()!=bmp->width()
         ||yuv->height()!=bmp->height())
        {
            delete final_bmp;
            delete yuv;
            final_bmp=nullptr;
            yuv=nullptr;
        }
    }

    if(!yuv)
    {
        yuv=new Bitmap4f(new BitmapDataAlloc<color4f>(bmp->width(),bmp->height()));
        final_bmp=new Bitmap4b(new BitmapDataAlloc<color4b>(bmp->width(),bmp->height()));
    }

    RGB2YCbCr(yuv,bmp);

    color4f result;
    color4b *tp=final_bmp->data();

    CutoffColor=ColorFeathering-Cutoff;

    for(int y=0;y<bmp->height();y++)
        for(int x=0;x<bmp->width();x++)
        {
            compute_pixel(tp,x,y);
            ++tp;
        }

    return final_bmp;
}