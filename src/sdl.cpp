//
//  sdl.cpp
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#include <stdio.h>
#include "sdl.h"

bool InitSDL()
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Instagram Effects", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
        else
        {
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
	}

	return success;
}

SDL_Surface* loadImageToSurface(const char* imageFilePath)
{
    SDL_Surface* loadedSurface = IMG_Load(imageFilePath);
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", imageFilePath, IMG_GetError());
    }
    
    return loadedSurface;
}

SDL_Surface* loadImageToSurface(int width, int height, int bits_per_pixel, void* pixels)
{
    int pitch = width * bits_per_pixel / BITS_IN_A_BYTE;
    SDL_Surface* loadedSurface = SDL_CreateRGBSurfaceFrom(pixels, width, height, bits_per_pixel, pitch, 0, 0, 0, 0); //default masks
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %p! SDL_image Error: %s\n", pixels, IMG_GetError());
    }
    
    return loadedSurface;
}

SDL_Surface* optimizeSurface(SDL_Surface* oldSurface)
{
    SDL_Surface* optimizedSurface = NULL;
    
    optimizedSurface = SDL_ConvertSurface(oldSurface, gScreenSurface->format, NULL);
    if( optimizedSurface == NULL )
    {
        printf("Unable to optimize image! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        printf("Successdully optimized image with %d Bits Per Pixel\n", optimizedSurface->format->BitsPerPixel);
    }
    SDL_FreeSurface(oldSurface);
    
    return optimizedSurface;
}

bool LoadImage(const char* imageFilePath)
{    
    gHelloWorld = loadImageToSurface(imageFilePath);
    if(gHelloWorld == NULL)
    {
        return false;
    }

    printf("Image %s loaded successfuly!\n", imageFilePath);
    return true;
}

bool LoadImage(int width, int height, int bits_per_pixel, void* pixels)
{
    gHelloWorld = loadImageToSurface(width, height, bits_per_pixel, pixels);
    if(gHelloWorld == NULL)
    {
        return false;
    }

    printf("Image from pixels loaded successfuly!\n");
    return true;
}

void GetScreenSurfaceImageInfo(int* width, int* height, int* bits_per_pixel, void **pixels)
{
    char* cPixels = (char*)gHelloWorld->pixels;
    *pixels = cPixels;
    
    *width  = gHelloWorld->w;
    *height = gHelloWorld->h;
    *bits_per_pixel  = gHelloWorld->format->BitsPerPixel;
}

void editSurfacePixels(SDL_Surface* surface)
{
    int bpp = surface->format->BytesPerPixel;
    for (int x = 0; x < surface->w; ++x)
    {
        for (int y = 0; y < surface->h; ++y)
        {
            // TODO: bpp switch
            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
            *p = *p + 100;
        }
    }
}

void EditImagePixels()
{
    editSurfacePixels(gHelloWorld);
}

void UpdateScreen()
{
    if(SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL) != 0)
    {
        printf("Unable to blit surface! SDL Error: %s\n",  SDL_GetError());
    }
    
    if(SDL_UpdateWindowSurface(gWindow) != 0)
    {
        printf("Unable to update window surface! SDL Error: %s\n",  SDL_GetError());
    }
    
    printf("Screen updated!\n");
}

void Wait(int delayInMiliseconds)
{
    SDL_Delay(delayInMiliseconds);
}

void CloseSDL()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int TestSDL()
{
    if(!InitSDL())
    {
        printf("Failed to initialize SDL!\n");
        return 1;
    }
    
    if(!LoadImage("data/hello_world.png"))
    {
        printf("Failed to load image!\n");
        return 1;
    }
    
    UpdateScreen();
    Wait(2000);
    
    EditImagePixels();
    
    UpdateScreen();
    Wait(2000);
    
    CloseSDL();
    
    return 0;
}