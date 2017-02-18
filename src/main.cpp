//
//  main.cpp
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#include "main.h"

int TestImageKernel(const char* filepath, const char* kernalName)
{
    int width, height, bits_per_pixel;

    void *inputPixels, *outputPixels;
    
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
    Wait(1000);
    
    if (InitOpenCL() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    GetScreenSurfaceImageInfo(&width, &height, &bits_per_pixel, &inputPixels);
    outputPixels = malloc(sizeof(unsigned int) * width * height * bits_per_pixel * BITS_IN_A_BYTE);
    
    RunOpenCLKernel(filepath, kernalName, width, height, inputPixels, outputPixels);
    
    if(!LoadImage(width, height, bits_per_pixel, outputPixels))
    {
        printf("Failed to load image from pixels!\n");
        return 1;
    }
    
    UpdateScreen();
    Wait(2000);
   
    if(!LoadImage(width, height, bits_per_pixel, inputPixels))
    {
        printf("Failed to load image from pixels!\n");
        return 1;
    }
    
    UpdateScreen();
    Wait(1000);
    
    RunOpenCLKernel("src/brightness.cl", "brightness", width, height, inputPixels, outputPixels);
    
    if(!LoadImage(width, height, bits_per_pixel, outputPixels))
    {
        printf("Failed to load image from pixels!\n");
        return 1;
    }
    
    UpdateScreen();
    Wait(2000);

    
    if (CleanUpOpenCL() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    CloseSDL();
    
    free(outputPixels);

    return 0;
}

int TestConvolutionKernal()
{
    int width, height, bits_per_pixel;
    
    void *inputPixels, *outputPixels;
    
    convolutionFilter filter;
    
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
    Wait(1000);
    
    if (InitOpenCL() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    GetScreenSurfaceImageInfo(&width, &height, &bits_per_pixel, &inputPixels);
    outputPixels = malloc(sizeof(unsigned int) * width * height * bits_per_pixel * BITS_IN_A_BYTE);
    
    filter.size = 3;
    filter.data = (float*)malloc(sizeof(float) * filter.size * filter.size);
    
    filter.data[0] = -1;
    filter.data[1] = -1;
    filter.data[2] = -1;
    
    filter.data[3] = -1;
    filter.data[4] = 8;
    filter.data[5] = -1;
    
    filter.data[6] = -1;
    filter.data[7] = -1;
    filter.data[8] = -1;
    
    RunConvolutionOpenCLKernel(width, height, inputPixels, outputPixels, filter);
    
    if(!LoadImage(width, height, bits_per_pixel, outputPixels))
    {
        printf("Failed to load image from pixels!\n");
        return 1;
    }
    
    UpdateScreen();
    Wait(3000);
    
    if (CleanUpOpenCL() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    
    CloseSDL();
    
    free(outputPixels);
    free(filter.data);
    
    return 0;

}

int main(int argc, char* args[])
{
    //TestSDL();
    //HelloWorldOpenCL();
    //TestImageKernel("src/grayscale.cl", "grayscale");
    //TestImageKernel("src/brightness.cl", "brightness");
    
    //TestImageKernel("src/threshold.cl", "threshold");
    TestConvolutionKernal();
    
    return 0;
}
