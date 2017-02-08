//
//  main.cpp
//  InstagramEffects
//
//  Created by Viktor Ketipov
//  Copyright (c) 2017 Viktor Ketipov. All rights reserved.
//

#include "main.h"

int main(int argc, char* args[])
{
    if(!InitSDL())
    {
        printf("Failed to initialize SDL!\n");
        return 1;
    }
    
    if(!LoadImage("data/hello_world.png"))
    {
        printf("Failed to load image!\n");
        return 2;
    }
    
    UpdateScreen();
    Wait(2000);
    
    EditImagePixels();
    
    UpdateScreen();
    Wait(2000);
    
    CloseSDL();
    
    return 0;
}
