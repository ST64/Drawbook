CC=g++
CFLAGS=-O3 -s 
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lm -L.
SRC=drawbook.c
EXE=Drawbook_D
AR=ar
windows: $(EXE) drawbook
	echo YAY!
linux: Drawbook2_D
	mv Drawbook2_D Drawbook
	echo YAY!
Drawbook2_D: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o Drawbook2_D
all: $(EXE) drawbook
	echo YAY!
$(EXE): $(SRC)
	$(CC) $(CFLAGS) -lmingw32 $(LDFLAGS) $(SRC) -o $(EXE) -lmingw32
drawbook: $(EXE)
	mv $(EXE) ../Drawbook.exe
install: $(EXE)
	cp Drawbook /usr/local/bin
clean:
	rm -fr *.o $(EXE) Drawbook2_D ../Drawbook
nsis:
	makensis Drawbook2.nsi
	makensis Drawbook3.nsi
