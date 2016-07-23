#include <SDL_colorpick.h>
#include <png.h>
#include <zlib.h>
Uint32 ReadPixel(SDL_Surface *surface, Sint16 x, Sint16 y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
void DrawPixel(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
/*SDL_Color color_wheel(SDL_Surface *s){
    SDL_Surface *ss=NULL;
	ss=IMG_Load("colorwheel.png");
    SDL_Surface *sbackup=SDL_CreateRGBSurface(s->flags, s->w, s->h, s->format->BitsPerPixel,s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask);
    SDL_Surface *sbackup2=SDL_CreateRGBSurface(s->flags, s->w, s->h, s->format->BitsPerPixel,s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask);
	if (SDL_BlitSurface(s,NULL,sbackup,NULL)<0){printf("FAIL!!!");}
	if (!ss){printf("FAIL!");}
	if (!sbackup){printf("FAIL!!");}
	printf("%s",IMG_GetError());
	SDL_Rect t;
	t.w=ss->w;
	t.h=ss->h;
	t.x=0;
	t.y=0;
    if (SDL_BlitSurface(ss,NULL,s,&t)<0){printf("FAIL!!!");}
    SDL_Event Event2;
    uint8_t r1=1;
    SDL_Color ll;
	SDL_Flip(s);
	SDL_Delay(1000);
    while ((SDL_PollEvent(&Event2))&&(r1==1)){
		if (SDL_BlitSurface(ss,NULL,s,&t)<0){printf("FAIL!!!");}
		SDL_Flip(s);
        if (Event2.type==SDL_MOUSEBUTTONDOWN){
			if (SDL_BlitSurface(s,NULL,sbackup2,NULL)<0){printf("FAIL!!!");}
			SDL_LockSurface(sbackup2);
			SDL_Delay(10);
            Uint32 t=ReadPixel(sbackup2,Event2.button.x,Event2.button.y);
			SDL_UnlockSurface(sbackup2);
            Uint8 r2=0;
            Uint8 g2=0;
            Uint8 b2=0;
            SDL_GetRGB(t, s->format,&r2, &g2, &b2);
            ll.r=r2;
            ll.g=g2;
            ll.b=b2;
            r1=0;
        }else{
			SDL_Delay(10);
		}
    }
    SDL_BlitSurface(sbackup,NULL,s,NULL);
	SDL_FreeSurface(ss);
	SDL_FreeSurface(sbackup);
	SDL_FreeSurface(sbackup2);
	printf("%s\n",IMG_GetError());
    return ll;
}*/