#include<iostream>
#include"Chroma.h"

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

    ChromaBitmap *cb=new ChromaBitmap();

    //���ÿ�ͼ��ɫ
    {
        color4b chroma_color;

        chroma_color.r=130;
        chroma_color.g=195;
        chroma_color.b=101;

        cb->SetChromaColor(chroma_color);

        cb->SetCutoff(0.125);
        cb->SetColorFeathering(0.15);
    }

    Bitmap4b *result=cb->Chroma(bmp);               //��BMPͼƬ���п�ͼ�����ؿ�ͼ�Ľ�����µ�Bitmap��

    if(!SaveToTGA(argv[1],result))
    {
        std::cerr<<"Save TGA failed!"<<std::endl;
    }
    
    delete cb;
    delete bmp;

    return 0;
}
