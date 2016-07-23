CC=clang
CFLAGS=-O3 -s `sdl-config --cflags` -I/opt/local/include -I/opt/local/include/SDL -Wall -Wextra -ansi -Wno-switch
LDFLAGS= -L/opt/local/lib  `sdl-config --libs` -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx -lm -L. -lSDL_image
SRC=drawbook.c
EXE=Drawbook_D
AR=ar
windows: $(EXE) drawbook
	echo YAY!
linux: Drawbook2_D
	mv Drawbook2_D Drawbook
	echo YAY!
Drawbook2_D: $(SRC) libSDL_Button.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o Drawbook2_D -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx -I/usr/include/ -I/usr/local/include -I. -L. -lSDL_Button -lSDL_image -lpng -lz -lSDL
all: $(EXE) drawbook
	echo YAY!
$(EXE): $(SRC) libSDL_Button.a
	$(CC) $(CFLAGS) -lmingw32 $(LDFLAGS) $(SRC) -o $(EXE) -lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx -I/usr/include -I/usr/local/include -I. -L. -lSDL_Button -lSDL_image -lpng -lz -lSDL
drawbook: $(EXE)
	mv $(EXE) ../Drawbook.exe
dogeboard: libSDL_Button.a dogeboard.c
	$(CC) $(CFLAGS) $(LDFLAGS) dogeboard.c -o ../dogeboard.exe -I/usr/local/include/SDL -D_GNU_SOURCE=1 -Dmain=SDL_main -L/usr/local/lib -lmingw32 -lSDLmain -lSDL -mwindows -lSDL_ttf -I. ../libSDL_Button.a -lSDL_mixer
install: $(EXE)
	cp Drawbook /usr/local/bin
libSDL_Button.a: SDL_Button.c SDL_Button.h SDL_colorpick.c SDL_colorpick.h
	$(CC) $(CFLAGS) SDL_Button.c -o SDL_Button.o -c -O3 -s -I/usr/include -I/usr/local/include -I. 
	$(CC) $(CFLAGS) -I/include -I/usr/include -I/usr/local/include -c SDL_colorpick.c -o SDL_colorpick.o -I.
	$(CC) $(CFLAGS) -I/include -I/usr/include -I/usr/local/include -c savepng.c -o pngfuncs.o -I.
	$(AR) rcs libSDL_Button.a SDL_Button.o SDL_colorpick.o pngfuncs.o
clean:
	rm -fr libSDL_Button.a *.o $(EXE) Drawbook2_D ../Drawbook
nsis:
	makensis Drawbook2.nsi
	makensis Drawbook3.nsi
