//
//  sdl.h
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#ifndef InstagramEffects_sdl_h
#define InstagramEffects_sdl_h

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static SDL_Window* gWindow = NULL;
static SDL_Surface* gScreenSurface = NULL;
static SDL_Surface* gHelloWorld = NULL;

bool InitSDL();

//SDL_Surface* loadImageToSurface(const char* imageFilePath);

bool LoadImage(const char* imageFilePath);

//void editSurfacePixels(SDL_Surface* surface);

void EditImagePixels();

void UpdateScreen();

void Wait(int delayInMiliseconds);

void CloseSDL();

int TestSDL();

#endif
