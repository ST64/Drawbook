#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_Button.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
uint8_t channels_free[8];
uint8_t running=1;
int main(int argc, char *argv[]){
	argc=argc;
	if (SDL_Init( SDL_INIT_EVERYTHING ) == -1){
        return 1;
    }
	if(TTF_Init()==-1) {
	printf("TTF_Init: %s\n", TTF_GetError());
	exit(2);
    }
	Mix_Init(MIX_INIT_OGG);
	SDL_Color color99={0,0,0,0};
	SDL_Button_t *buttons = (SDL_Button_t *)(malloc(9*sizeof(SDL_Button_t)));
	SDL_Button_t *tmp = (SDL_Button_t *)(malloc(sizeof(SDL_Button_t)));
	SDL_Rect c;
	c.x=(590/9);
	c.h=50;
	c.y=460;
	c.w=59;
	SDL_Surface *screen = SDL_SetVideoMode( 945, 590, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 255,255,255));
	SDL_Flip(screen);
	SDL_Surface * ssss[9];
	SDL_Event Event;
	TTF_Font *font;
    font=TTF_OpenFont("TTF\\Drawbook.ttf",25);
	ssss[0]= TTF_RenderText_Solid(font,"A1",color99);
	ssss[1]=TTF_RenderText_Solid(font,"B1",color99);
	ssss[2]=TTF_RenderText_Solid(font,"C1",color99);
	ssss[3]=TTF_RenderText_Solid(font,"D1",color99);
    ssss[4]=TTF_RenderText_Solid(font,"E1",color99);
    ssss[5]=TTF_RenderText_Solid(font,"F1",color99);
    ssss[6]=TTF_RenderText_Solid(font,"G1",color99);
    ssss[7]=TTF_RenderText_Solid(font,"A2",color99);
	uint8_t i=0;
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	Mix_Chunk *Loaded[13];
	Mix_Chunk *l1=Mix_LoadWAV("Ogg\\A1.ogg");
	Loaded[0] = l1;
	l1=Mix_LoadWAV("Ogg\\B1.ogg");
	Loaded[1] = l1;
	l1=Mix_LoadWAV("Ogg\\C2.ogg");
	Loaded[2] = l1;
	l1=Mix_LoadWAV("Ogg\\D2.ogg");
	Loaded[3] = l1;
	l1=Mix_LoadWAV("Ogg\\E2.ogg");
	Loaded[4] = l1;
	l1=Mix_LoadWAV("Ogg\\F2.ogg");
	Loaded[5] = l1;
	l1=Mix_LoadWAV("Ogg\\G2.ogg");
	Loaded[6] = l1;
	l1=Mix_LoadWAV("Ogg\\A2.ogg");
	Loaded[7] = l1;
	l1=Mix_LoadWAV("Ogg\\B2.ogg");
	Loaded[8]= l1;
	l1=Mix_LoadWAV("Ogg\\C3.ogg");
	Loaded[9]=l1;
	l1=Mix_LoadWAV("Ogg\\D3.ogg");
	Loaded[10]=l1;
	l1=Mix_LoadWAV("Ogg\\E3.ogg");
	Loaded[11]=l1;
	l1=Mix_LoadWAV("Ogg\\F3.ogg");
	Loaded[12]=l1;
	SDL_WM_SetCaption("DogeBoard", "accessories-text-editor");
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 255,255,255));
	for (i=0;i<8;i++){
			tmp = SDL_Button(screen,c.x,c.y,c.w,c.h);
			buttons[i]=*tmp;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format,68,68,68));
			SDL_BlitSurface(ssss[i],NULL,screen,&c);
			c.x+=(590/9);
	}
	Mix_Volume(-1,MIX_MAX_VOLUME);
	Mix_AllocateChannels(16);
	while (running==1){
		c.x=(590/9);
		SDL_Flip(screen);
		while (SDL_PollEvent(&Event)){
			switch (Event.type){
				case SDL_QUIT:;
					running=0;
					break;
				case SDL_MOUSEBUTTONDOWN:;
					for (i=0;i<8;i++){
						if (SDL_Button_mouse_down(&buttons[i],&Event)){
							Mix_PlayChannel(i+1,Loaded[i],0);
						}
					}
					SDL_Flip(screen);
					break;
				case SDL_KEYDOWN:;
					if (Event.key.keysym.sym==SDLK_1){
						Mix_PlayChannel(1,Loaded[0],0);
					}else if (Event.key.keysym.sym==SDLK_2){
						Mix_PlayChannel(2,Loaded[1],0);
					}else if (Event.key.keysym.sym==SDLK_3){
						Mix_PlayChannel(3,Loaded[2],0);
					}else if (Event.key.keysym.sym==SDLK_4){
						Mix_PlayChannel(4,Loaded[3],0);
					}else if (Event.key.keysym.sym==SDLK_5){
						Mix_PlayChannel(5,Loaded[4],0);
					}else if (Event.key.keysym.sym==SDLK_6){
						Mix_PlayChannel(6,Loaded[5],0);
					}else if (Event.key.keysym.sym==SDLK_7){
						Mix_PlayChannel(7,Loaded[6],0);
					}else if (Event.key.keysym.sym==SDLK_8){
						Mix_PlayChannel(8,Loaded[7],0);
					}else if (Event.key.keysym.sym==SDLK_9){
						Mix_PlayChannel(9,Loaded[8],0);
					}else if (Event.key.keysym.sym==SDLK_0){
						Mix_PlayChannel(10,Loaded[9],0);
					}else if (Event.key.keysym.sym==SDLK_MINUS){
						Mix_PlayChannel(11,Loaded[10],0);
					}else if (Event.key.keysym.sym==SDLK_EQUALS){
						Mix_PlayChannel(12,Loaded[11],0);
					}else if (Event.key.keysym.sym==SDLK_BACKSPACE){
						Mix_PlayChannel(13,Loaded[12],0);
					}
					break;
				default:;
					break;
			}
		}
		SDL_Flip(screen);
	}
	Mix_HaltChannel(-1);
	Mix_CloseAudio();
	Mix_FreeChunk(Loaded[0]);
	Mix_FreeChunk(Loaded[1]);
	Mix_FreeChunk(Loaded[2]);
	Mix_FreeChunk(Loaded[3]);
	Mix_FreeChunk(Loaded[4]);
	Mix_FreeChunk(Loaded[5]);
	Mix_FreeChunk(Loaded[6]);
	Mix_FreeChunk(Loaded[7]);
	Mix_FreeChunk(Loaded[8]);
	Mix_FreeChunk(Loaded[9]);
	Mix_FreeChunk(Loaded[10]);
	Mix_FreeChunk(Loaded[11]);
	Mix_FreeChunk(Loaded[12]);
	Mix_FreeChunk(Loaded[13]);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}