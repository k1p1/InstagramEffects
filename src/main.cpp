//
//  main.cpp
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#include "main.h"

int TestGrayscale()
{
    int width, height, bytes_per_pixel;
    void* inputPixels, *outputPixels;
    
    if(!InitSDL())
    {
        printf("Failed to initialize SDL!\n");
        return 1;
    }

    if(!LoadImage("data/colours.png"))
    {
        printf("Failed to load image!\n");
        return 1;
    }

    UpdateScreen();
    Wait(2000);

    GetScreenSurfaceImageInfo(&width, &height, &bytes_per_pixel, &inputPixels);
    outputPixels = malloc(sizeof(unsigned int) * width * height * 4);
    GrayscaleOpenCL(width, height, bytes_per_pixel, inputPixels, outputPixels);
    
    if(!LoadImage(width, height, bytes_per_pixel, outputPixels))
    {
        printf("Failed to load image from pixels!\n");
        return 1;
    }

    UpdateScreen();
    Wait(2000);

    CloseSDL();

    return 0;
}

int main(int argc, char* args[])
{
    //TestSDL();
    //HelloWorldOpenCL();
    TestGrayscale();
    
    return 0;
}
