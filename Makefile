CC=g++
CFLAGS=-O3 -s -I "C:\Program Files (x86)\SFML\include" -w
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lm -L. -L "C:\Program Files (x86)\SFML\lib" -mwindows
SRC=src\drawbook2.cpp
EXE=Drawbook.exe
AR=ar
windows: $(EXE) drawbook
	echo YAY!
linux: Drawbook2_D
	mv Drawbook2_D Drawbook
	echo YAY!
Drawbook2_D: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o Drawbook2_D $(LDFLAGS)
all: $(EXE) drawbook
	echo YAY!
$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(EXE) -lmingw32 $(LDFLAGS)
drawbook: $(EXE)

install: $(EXE)
	cp Drawbook /usr/local/bin
clean:
	rm -fr *.o $(EXE) Drawbook2_D ../Drawbook
nsis:
	makensis Drawbook2.nsi
	makensis Drawbook3.nsi
