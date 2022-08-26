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



    return 0;
}
