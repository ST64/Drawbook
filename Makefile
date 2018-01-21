CXX = $(shell fltk-config --cxx)
CXXFLAGS = $(shell fltk-config --cxxflags) -O2 -std=c++11 -flto
LINKFLTK = $(shell fltk-config --use-gl --ldflags) -flto
POSTBUILD = fltk-config --post

all: Drawbook

src/drawbook2.o: src/drawbook2.cpp
	$(CXX) -o src/drawbook2.o -c $< $(CXXFLAGS)

Drawbook: src/drawbook2.o
	$(CXX) -o $@ src/drawbook2.o $(LINKFLTK)
	$(POSTBUILD) $@

clean:
	rm -fr *.o src/*.o Drawbook
