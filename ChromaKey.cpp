#include <iostream>
#include"bitmap2d.h"

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

public:

    ChromaBitmap(Bitmap4b *b)
    {
        bmp=b;

        yuv=new Bitmap4f(new BitmapDataAlloc<color4f>(bmp->width(),bmp->height()));

        RGB2YCbCr(yuv,bmp);

        final_bmp=new Bitmap4b(new BitmapDataAlloc<color4b>(bmp->width(),bmp->height()));

        Cutoff=0.2*0.2;
        ColorFeathering=0.33*0.33;
        MaskFeathering=1.0;
        Sharpening=0.5;
        Despill=1.0;
        DespillLuminanceAdd=0.2;
    }

    ~ChromaBitmap()
    {
        delete yuv;
        delete final_bmp;
    }

    void SetChromaColor(const color4b &color){RGB2YCbCr(chroma_color,color);}
    void SetCutoff(double value){Cutoff=value*value;}
    void SetColorFeathering(double cf){ColorFeathering=cf*cf;}
    void SetMaskFeathering(double mf){MaskFeathering=mf;}
    void SetSharpening(double s){Sharpening=s;}
    void SetDespill(float d){Despill=d;}
    void SetDespillLumianceAdd(float dla){DespillLuminanceAdd=dla;}

    const color4f tex2d(const int x,const int y)const
    {
        color4b rgb;

        bmp->get(x,y,rgb);

        return color4f( float(rgb.r)/255.0f,
                        float(rgb.g)/255.0f,
                        float(rgb.b)/255.0f,
                        1.0);
    }

    double masked(const int x,const int y)
    {
        color4f color;

        yuv->get(x,y,color);

        double temp=(chroma_color.cb-color.cb)*(chroma_color.cb-color.cb)
                   +(chroma_color.cr-color.cr)*(chroma_color.cr-color.cr);

        if(temp<Cutoff)return 0;
        if(temp<ColorFeathering)return (temp-Cutoff)/CutoffColor;
        return 1;
    }

    void compute_pixel(color4b *out_color,const int x,const int y)
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

    Bitmap4b *Run()
    {
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
};//class ChromaBitmap

int main(int argc,char **argv)
{
    if(argc<3)
    {
        std::cout<<"Example: ChromaKey out.tga in.tga"<<std::endl;
        return 0;
    }

    std::cout<<"in:  "<<argv[2]<<std::endl;
    std::cout<<"out: "<<argv[1]<<std::endl;

    Bitmap4b *bmp=LoadFromTGA(argv[2]);

    if(!bmp)
    {
        std::cerr<<"Load TGA failed!"<<std::endl;
        return 1;
    }

    ChromaBitmap *cb=new ChromaBitmap(bmp);

    //ÉèÖÃ¿ÙÍ¼ÑÕÉ«
    {
        color4b chroma_color;

        chroma_color.r=130;
        chroma_color.g=195;
        chroma_color.b=101;

        cb->SetChromaColor(chroma_color);

        cb->SetCutoff(0.125);
        cb->SetColorFeathering(0.15);
    }

    Bitmap4b *result=cb->Run();

    if(!SaveToTGA(argv[1],result))
    {
        std::cerr<<"Save TGA failed!"<<std::endl;
    }
    
    delete cb;
    delete bmp;

    return 0;
}
