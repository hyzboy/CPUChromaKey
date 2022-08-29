#pragma once
#include"bitmap2d.h"

/**
 * Bitmap¿ÙÍ¼¹¤¾ßÀà
 */
class ChromaBitmap
{
    Bitmap4b *bmp;
    Bitmap4f *yuv;

    Bitmap4b *final_bmp;

    color4f chroma_color;
    double Cutoff;
    double ColorFeathering;
    double CutoffColor;

    double MaskFeathering;
    double Sharpening;
    float Despill;
    float DespillLuminanceAdd;

private:

    const color4f tex2d(const int x,const int y)const;
    double masked(const int x,const int y);

    void compute_pixel(color4b *out_color,const int x,const int y);

public:

    ChromaBitmap();
    ~ChromaBitmap();

    void SetChromaColor(const color4b &color){RGB2YCbCr(chroma_color,color);}
    void SetCutoff(double value){Cutoff=value*value;}
    void SetColorFeathering(double cf){ColorFeathering=cf*cf;}
    void SetMaskFeathering(double mf){MaskFeathering=mf;}
    void SetSharpening(double s){Sharpening=s;}
    void SetDespill(float d){Despill=d;}
    void SetDespillLumianceAdd(float dla){DespillLuminanceAdd=dla;}

public:

    Bitmap4b *Chroma(Bitmap4b *);
};//class ChromaBitmap
