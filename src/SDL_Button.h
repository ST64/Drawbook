

#include "SDL/SDL.h"
#include <stdbool.h>


typedef struct {
	SDL_Surface * internal_surface;
	SDL_Rect * location_and_size;
} SDL_Button_t;


SDL_Button_t * SDL_Button(SDL_Surface * surface, int x_location, int y_location, int button_width, int button_h);


void SDL_Button_free(SDL_Button_t * button);


bool SDL_Button_mouse_down(SDL_Button_t * button, SDL_Event * e);


bool SDL_Button_mouse_up(SDL_Button_t * button, SDL_Event * e);


bool SDL_Button_mouse_over(SDL_Button_t * button, SDL_Event * e);


bool SDL_Button_mouse_out(SDL_Button_t * button, SDL_Event * e);