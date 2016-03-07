#ifndef SDL_COLORPICK_H
#define SDL_COLORPICK_H 1
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <SDL_image.h>
Uint32 ReadPixel(SDL_Surface *surface, Sint16 x, Sint16 y);
void DrawPixel(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 pixel);/*
SDL_Color color_wheel(SDL_Surface *screen);*/
#endif