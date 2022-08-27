#include <iostream>
#include"bitmap2d.h"

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

    if(!SaveToTGA(argv[1],bmp))
    {
        std::cerr<<"Save TGA failed!"<<std::endl;
    }

    delete bmp;

    return 0;
}
