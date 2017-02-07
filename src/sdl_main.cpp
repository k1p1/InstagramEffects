#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow( "Instagram Effects", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadImage(const char* imageFilePath)
{
	bool success = true;
    
	gHelloWorld = SDL_LoadBMP( imageFilePath );
	if( gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", imageFilePath, SDL_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadImage("data/hello_world.bmp") )
		{
			printf( "Failed to load image!\n" );
		}
		else
		{
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
			
			SDL_UpdateWindowSurface( gWindow );

			SDL_Delay( 2000 );
		}
	}

	close();

	return 0;
}