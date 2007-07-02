CC=g++

CXXFLAGS=-D__LINUX__ $(shell sdl-config --cflags) -g
LDFLAGS=$(shell sdl-config --libs) -lSDL_image -L../common/ -ltk4common

TARGETS = main.o

all: $(TARGETS)
	g++ $(TARGETS) $(CXXFLAGS) $(LDFLAGS) -o efpconv

clean:
	rm -f $(TARGETS)

main.o: ../common/CGraphicsBuffer.h ../common/CPalette.h ../common/error.h ../common/defines.h ../common/CRect.h ../common/linux.h ../common/windows.h ../common/CCoord.h ../common/CMath.h
