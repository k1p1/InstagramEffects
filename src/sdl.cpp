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
		gWindow = SDL_CreateWindow("Instagram Effects", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

static SDL_Surface* loadImageToSurface(const char* imageFilePath)
{
    SDL_Surface* optimizedSurface = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load(imageFilePath);
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", imageFilePath, IMG_GetError());
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
        if( optimizedSurface == NULL )
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", imageFilePath, SDL_GetError());
        }
        else
        {
            printf("Successdully optimezed image %s with %d Bytes Per Pixel\n", imageFilePath, optimizedSurface->format->BytesPerPixel);
        }
        SDL_FreeSurface(loadedSurface);
    }
    
    return optimizedSurface;
}

bool LoadImage(const char* imageFilePath)
{
    bool success = true;
    
    gHelloWorld = loadImageToSurface(imageFilePath);
    if(gHelloWorld == NULL)
    {
        printf("Failed to load image!\n");
        success = false;
    }
    
    return success;
}

void editSurfacePixels(SDL_Surface* surface)
{
    int bpp = surface->format->BytesPerPixel;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        for (int y = 0; y < SCREEN_HEIGHT; ++y)
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