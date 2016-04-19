#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>
#include <SDL_Button.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <savepng.h>
#include <SDL_colorpick.h>
#define Init_IMG() IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
uint8_t legacy=0;
uint8_t running = 1;
uint8_t drawing = 0;
int x1,y,x2,y2;
int width = 1, color=0;
int r[]={0,255,255,0,0,128, 255}; /* black, red, yellow, green 3, deepskyblue, gray, white*/
int g[]={0,0,255,205,199,128,255};
int b[]={0,0,0,0,255,128,255};
static int s=0;
uint8_t buffindex=0;
uint8_t line=1;
char savescreen[65];
char bmpname[100];
char buffer2[200];
uint8_t clear=0;
int screenh;
int screenw;
uint8_t word=0;
uint8_t shift=0;
uint8_t cntr=0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
int rmask = 0xff000000;
int gmask = 0x00ff0000;
int bmask = 0x0000ff00;
int amask = 0x000000ff;
#else
int rmask = 0x000000ff;
int gmask = 0x0000ff00;
int bmask = 0x00ff0000;
int amask = 0xff000000;
#endif
char getmod(char keyvalue,SDLMod modvalue){
    char d=0;
    if ((keyvalue>=97)&&(keyvalue<=122)&&((modvalue==KMOD_CAPS)||(modvalue==KMOD_RSHIFT)||(modvalue==KMOD_LSHIFT))){
	d=((keyvalue-97)+65);
    }else{
	d=keyvalue;
    }
    return d;
}
char getkey(SDLKey keymapvalue){
    char c=0;
    switch (keymapvalue){
    case SDLK_a:;
	    c='a';
	    break;
	case SDLK_b:;
	    c='b';
	    break;
	case SDLK_c:;
	    c='c';
	    break;
	case SDLK_d:;
	    c='d';
	    break;
	case SDLK_e:;
	    c='e';
	    break;
	case SDLK_f:;
	    c='f';
	    break;
	case SDLK_g:;
	    c='g';
	    break;
	case SDLK_h:;
	    c='h';
	    break;
	case SDLK_i:;
	    c='i';
	    break;
	case SDLK_j:;
	    c='j';
	    break;
	case SDLK_k:;
	    c='k';
	    break;
	case SDLK_l:;
	    c='l';
	    break;
	case SDLK_m:;
	    c='m';
	    break;
	case SDLK_n:;
	    c='n';
	    break;
	case SDLK_o:;
	    c='o';
	    break;
	case SDLK_p:;
	    c='p';
	    break;
	case SDLK_q:;
	    c='q';
	    break;
	case SDLK_r:;
	    c='r';
	    break;
	case SDLK_s:;
	    c='s';
	    break;
	case SDLK_t:;
	    c='t';
	    break;
	case SDLK_u:;
	    c='u';
	    break;
	case SDLK_v:;
	    c='v';
	    break;
	case SDLK_w:;
	    c='w';
	    break;
	case SDLK_x:;
	    c='x';
	    break;
	case SDLK_y:;
	    c='y';
	    break;
	case SDLK_z:;
	    c='z';
	    break;
	case SDLK_SPACE:;
	    c=' ';
	    break;
	case SDLK_PERIOD:;
	    c='.';
	    break;
	case SDLK_AT:;
		c='@';
		break;
	case SDLK_HASH:;
		c='#';
		break;
	case SDLK_DOLLAR:;
		c='$';
		break;
	case SDLK_CARET:;
		c='^';
		break;
	case SDLK_AMPERSAND:;
		c='&';
		break;
	case SDLK_ASTERISK:;
		c='*';
		break;
	case SDLK_QUESTION:;
	    c='?';
	    break;
	case SDLK_COLON:;
	    c=':';
	    break;
	case SDLK_EXCLAIM:;
		c='!';
		break;
	case SDLK_LEFTPAREN:;
		c='(';
		break;
	case SDLK_RIGHTPAREN:;
		c=')';
		break;
	case SDLK_LEFTBRACKET:;
		c='[';
		break;
	case SDLK_RIGHTBRACKET:;
		c=']';
		break;
	case SDLK_BACKSLASH:;
		c='\\';
		break;
	case SDLK_UNDERSCORE:;
		c='_';
		break;
	case SDLK_SLASH:;
		c='/';
		if (shift==1){
			c='?';
		}
		break;
	default:;
		c='\0';
		break;
    }
    return c;
}
int main(int argc, char* argv[])
{
	argc=argc;
    FILE *textfile=fopen("filetext.txt","w");
    if (SDL_Init( SDL_INIT_VIDEO ) == -1){
        return 1;
    }
    if(TTF_Init()==-1) {
	printf("TTF_Init: %s\n", TTF_GetError());
	exit(2);
    }
    TTF_Font *font;
    font=TTF_OpenFont("Drawbook.ttf",25);
    uint32_t fontsize=TTF_FontHeight(font);
    SDL_Surface *screen = NULL;
    SDL_Rect textrect;
    SDL_Event Event;
    SDL_Rect wordrect;
    wordrect.x=0;
    wordrect.y=(fontsize+8)*line;
    screenh=590;
    screenw=945;
    screen = SDL_SetVideoMode( 945, 590, 8, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);
    SDL_Surface *buf= SDL_CreateRGBSurface(SDL_HWSURFACE, screenw, screenh, 8,rmask,gmask,bmask,amask);
    SDL_Color color99={0,0,0,0};
    if (screen == NULL){
        return 1;
    }
	SDL_Surface * ssss= TTF_RenderText_Solid(font,"New",color99);
	SDL_Surface * ssss2=TTF_RenderText_Solid(font,"Inc",color99);
	SDL_Surface * ssss3=TTF_RenderText_Solid(font,"Dec",color99);
	SDL_Surface * ssss4=TTF_RenderText_Solid(font,"Color",color99);
    SDL_Surface * ssss5=TTF_RenderText_Solid(font,"Save",color99);
    SDL_Surface * ssss6=TTF_RenderText_Solid(font,"Load",color99);
 	SDL_Rect c;
	c.x=0;
	c.y=0;
	c.h=50;
	c.w=80;
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 255,255,255));
	SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 68,68,68));
	SDL_Button_t *button1 = SDL_Button(screen,0,0,80,50);
	SDL_Button_t *button2 = SDL_Button(screen,80,0,80,50);
	SDL_Button_t *button3 = SDL_Button(screen,160,0,80,50);
	SDL_Button_t *button4 = SDL_Button(screen,240,0,160,50);
    SDL_Button_t *button5 = SDL_Button(screen,400,0,120,50);
    SDL_Button_t *button6 = SDL_Button(screen,520,0,120,50);
	SDL_BlitSurface(ssss,NULL,screen,&c);
    SDL_WM_SetCaption("Drawbook", "accessories-text-editor");
    SDL_Flip(screen);
    SDL_Surface *text_surface=TTF_RenderText_Solid(font," ",color99);
    while (running){
		if (legacy==0){
			c.x=0;
			c.y=0;
			c.h=50;
			c.w=80;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));
			c.x+=80;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));
			c.x+=80;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));
			c.x+=80;
			c.w+=80;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));
			c.w-=40;
			c.x+=160;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));
			c.x+=120;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));/*
			c.x+=120;
			c.w=80;
			SDL_FillRect(screen,&c,SDL_MapRGB(screen->format, 138,138,138));*/
			c.x=0;
			c.y=0;
			c.h=50;
			c.w=80;
			SDL_BlitSurface(ssss,NULL,screen,&c);
			c.x+=80;
			SDL_BlitSurface(ssss2,NULL,screen,&c);
			c.x+=80;
			SDL_BlitSurface(ssss3,NULL,screen,&c);
			c.x+=80;
			c.w+=80;
			SDL_BlitSurface(ssss4,NULL,screen,&c);
			c.w-=40;
			c.x+=160;
			SDL_BlitSurface(ssss5,NULL,screen,&c);
			c.x+=120;
			SDL_BlitSurface(ssss6,NULL,screen,&c);/*
        c.x+=120;
        c.w=80;
        SDL_BlitSurface(ssss7,NULL,screen,&c);*/
		}
		if (clear==1){
			SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 255,255,255));
			clear=0;
		}
        SDL_Flip(screen);
        while (SDL_PollEvent(&Event)){
            if (word==1){
		text_surface=TTF_RenderText_Solid(font,buffer2,color99);
		SDL_BlitSurface(text_surface,NULL,screen,&wordrect);
	    }
            SDL_Flip(screen);
            if (Event.type == SDL_QUIT){
                running = 0;
            }
            else if (Event.type == SDL_MOUSEBUTTONDOWN){
                drawing = 1;
                x1= Event.button.x;
                y = Event.button.y;
                x2 = x1;
                y2 = y;
				if (legacy==0){
					if (SDL_Button_mouse_down(button1,&Event)==1){
						clear=1;
					}
					if (SDL_Button_mouse_down(button2,&Event)==1){
						width++;
					}
					if (SDL_Button_mouse_down(button3,&Event)==1){
						width--;
					}
					if (SDL_Button_mouse_down(button4,&Event)==1){
						if (color<7){color++;}
						else{color=0;}
					}
					if (SDL_Button_mouse_down(button5,&Event)==1){
						if (s<99){
							s++;
						}else{
							s=0;
						}
						itoa(s,savescreen,10);
						strcat(bmpname,savescreen);
						strcat(bmpname,".png");
						SDL_SavePNG(screen,bmpname);
						memset(bmpname,0,99);
					}
					if (SDL_Button_mouse_down(button6,&Event)==1){
						itoa(s,savescreen,10);
						strcat(bmpname,savescreen);
						strcat(bmpname,".png");
						buf=IMG_Load(bmpname);
						SDL_BlitSurface(buf,NULL,screen,NULL);
						memset(bmpname,0,99);
					}
				}
            }
            else if (Event.type == SDL_MOUSEBUTTONUP){
                drawing = 0;
                x1 = Event.button.x;
                y = Event.button.y;
                x2 = x1;
                y2 = y;
            }
            else if (Event.type == SDL_MOUSEMOTION){
                x1= Event.motion.x;
                y=Event.motion.y;
                if (drawing == 1){
                    thickLineRGBA(screen, x1, y, x2,y2,width, r[color],g[color],b[color],255);
                }
                x2 = x1;
                y2 = y;
            }
            else if (Event.type == SDL_VIDEORESIZE){
                screenw=Event.resize.w;
                screenh=Event.resize.h;
                screen = SDL_SetVideoMode(Event.resize.w,Event.resize.h,8,SDL_HWSURFACE|SDL_RESIZABLE|SDL_DOUBLEBUF);
                SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,255,255,255));
                SDL_Flip(screen);
            }else if (Event.type == SDL_KEYUP){
				if ((Event.key.keysym.sym==SDLK_RSHIFT)||(Event.key.keysym.sym==SDLK_LSHIFT)){
					shift=0;
				}
			}
            else if (Event.type == SDL_KEYDOWN){
                switch (Event.key.keysym.sym){
                    case SDLK_ESCAPE:;
	      if ((word==0)||(cntr==19)){
                        	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 255,255,255));
	      }else{
		word=0;
		line=0;
	      }
                        break;
                    case SDLK_LEFT:;
			if ((word==0)||(cntr==1)){
                            if (color>0){color--;}
			}else{
			    buffindex--;
			}
                       	break;
                    case SDLK_RIGHT:;
			if (word==1){
  			    buffindex++;
			}else{
                            if (color<7){color++;}
                            break;
			}
		    case SDLK_UP:;
			width++;
			break;
		    case SDLK_DOWN:;
			width--;
			break;
		    case SDLK_0:;
			if ((word==0)||(cntr==1)){
			    running=0;
			}else{
			     buffer2[buffindex]='0';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_RIGHTPAREN);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_1:;
			if (word==1){
			     buffer2[buffindex]='1';
				 if (shift==1){
					buffer2[buffindex]='!';
				 }
			     buffindex++;
			}
			break;
		    case SDLK_2:;
			if (word==0){
				legacy=1;
				c.x=0;
				c.y=0;
				c.h=50;
				c.w=640;
				SDL_FillRect(screen,&c,SDL_MapRGB(screen->format,255,255,255));
			}
			if (word==1){
			    buffer2[buffindex]='2';
				if (shift==1){
					buffer2[buffindex]=getkey(SDLK_AT);
				}
			    buffindex++;
			}
			break;
		    case SDLK_3:;
			legacy=0;
			if (word==1){
			     buffer2[buffindex]='3';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_HASH);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_4:;
			if (word==1){
			     buffer2[buffindex]='4';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_DOLLAR);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_5:;
			if (word==1){
			     buffer2[buffindex]='5';
				 if (shift==1){
					buffer2[buffindex]='%';
				 }
			     buffindex++;
			}
			break;
		    case SDLK_6:;
			if (word==1){
			     buffer2[buffindex]='6';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_CARET);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_7:;
			if (word==1){
			     buffer2[buffindex]='7';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_AMPERSAND);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_8:;
			if (word==1){
			     buffer2[buffindex]='8';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_ASTERISK);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_9:;
			if (word==1){
			     buffer2[buffindex]='9';
				 if (shift==1){
					buffer2[buffindex]=getkey(SDLK_LEFTPAREN);
				 }
			     buffindex++;
			}
			break;
		    case SDLK_BACKSPACE:;
			buffindex--;
			buffer2[buffindex]=0;
			buffer2[buffindex+1]=0;
			wordrect.h=fontsize;
			wordrect.x=0;
			wordrect.y=fontsize*line;
			wordrect.w=screenw;
			SDL_FillRect(screen,&wordrect,SDL_MapRGB(screen->format,255,255,255));
			text_surface=TTF_RenderText_Solid(font,buffer2,color99);
			SDL_BlitSurface(text_surface,NULL,screen,&wordrect);
			break;
		    case SDLK_RETURN:;
			line++;
			wordrect.y+=fontsize;
			wordrect.x=0;
			fprintf(textfile,"%s",buffer2);
			for (buffindex=198;buffindex>0;buffindex--){
				buffer2[buffindex]=0;
			}
			buffer2[0]=0;
			break;
		    case SDLK_a:;
			if ((word==0)||(cntr==1)){
			    if (s<99){
			        s++;
			    }else{
			        s=0;
			    }
			    itoa(s,savescreen,10);
			    strcat(bmpname,savescreen);
			    strcat(bmpname,".bmp");
			    SDL_SaveBMP(screen,bmpname);
			    memset(bmpname,0,99);
			}else{
			    buffer2[buffindex]=getmod('a',Event.key.keysym.mod);
			    buffindex++;
			}
			break;
		    case SDLK_l:;
			if ((word==0)||(cntr==1)){
                itoa(s,savescreen,10);
    		    strcat(bmpname,savescreen);
			    strcat(bmpname,".bmp");
			    buf=SDL_LoadBMP(bmpname);
			    SDL_BlitSurface(buf,NULL,screen,NULL);
                memset(bmpname,0,99);
			}else{
                /*itoa(s,savescreen,10);
    		    strcat(bmpname,savescreen);
			    strcat(bmpname,".bmp");
			    buf=SDL_LoadBMP(bmpname);
			    SDL_BlitSurface(buf,NULL,screen,NULL);*/
			    buffer2[buffindex]=getmod('l',Event.key.keysym.mod);
			    buffindex++;
                memset(bmpname,0,99);
			}
			break;
		    case SDLK_w:;
			if ((word==0)||(cntr==1)){s++;}
			else{
			    buffer2[buffindex]=getmod('w',Event.key.keysym.mod);
			    buffindex++;
			}
			break;
		    case SDLK_s:;
			if ((word==0)||(cntr==1)){s--;}
			else{
			    buffer2[buffindex]=getmod('s',Event.key.keysym.mod);
			    buffindex++;
			}
			break;
		    case SDLK_h:;
			if ((word==0)||(cntr==1)){
			    textrect.x=0;
			    textrect.y=0;
			    text_surface=TTF_RenderText_Solid(font,"Welcome to Drawbook. Press Up or Down to change line size",color99);
			    SDL_BlitSurface(text_surface,NULL,screen,NULL);
			    textrect.y+=fontsize;
			    text_surface=TTF_RenderText_Solid(font,"Press W or S to change screenshot number, and A and L to",color99);
			    SDL_BlitSurface(text_surface,NULL,screen,&textrect);
			    text_surface=TTF_RenderText_Solid(font,"Save and load screenshots. Draw with the mouse",color99);
			    textrect.y+=fontsize;
			    SDL_BlitSurface(text_surface,NULL,screen,&textrect);
			    text_surface=TTF_RenderText_Solid(font,"Press O to switch to word processor and escape to exit it",color99);
				textrect.y+=fontsize;
				SDL_BlitSurface(text_surface,NULL,screen,&textrect);
				text_surface=TTF_RenderText_Solid(font,"Press 2 and 3 to hide/show buttons",color99);
				textrect.y+=fontsize;
				SDL_BlitSurface(text_surface,NULL,screen,&textrect);
				text_surface=TTF_RenderText_Solid(font,"More features COMING SOON...",color99);
				textrect.y+=fontsize;
				SDL_BlitSurface(text_surface,NULL,screen,&textrect);
			}else{
			    buffer2[buffindex]=getmod('h',Event.key.keysym.mod);
			    buffindex++;
			}
			break;
		    case SDLK_o:;
			if ((word==0)||(cntr==1)){
			    word=1;
			}else{
			    buffer2[buffindex]=getmod('o',Event.key.keysym.mod);
			    buffindex++;
			}
			break;
		    default:;
			if (word==1){
				if ((Event.key.keysym.sym==SDLK_RSHIFT)||(Event.key.keysym.sym==SDLK_LSHIFT)){
					shift=1;
				}else if ((Event.key.keysym.sym==SDLK_RCTRL)||(Event.key.keysym.sym==SDLK_LCTRL)){
					cntr=1;
				}else{
					buffer2[buffindex]=getkey(Event.key.keysym.sym);
					buffer2[buffindex]=getmod(buffer2[buffindex],Event.key.keysym.mod);
					buffindex++;
				}
			}
                        		break;
                }
            }
        }
    }/*
done:
    Pa_Terminate();*/
    SDL_FreeSurface(text_surface);
    SDL_FreeSurface(buf);
	SDL_FreeSurface(ssss);
	SDL_FreeSurface(ssss2);
	SDL_FreeSurface(ssss3);
	SDL_FreeSurface(ssss4);
	SDL_FreeSurface(button1->internal_surface);
	SDL_FreeSurface(button2->internal_surface);
	SDL_FreeSurface(button3->internal_surface);
	SDL_FreeSurface(button4->internal_surface);
    SDL_Quit();
    TTF_Quit();
	(void) argv;
    exit(0);
    return 0;
}