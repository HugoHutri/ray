CC = mingw32-g++
INCLUDES = -IC:/dev/SDL2/i686-w64-mingw32/include
CFLAGS = $(INCLUDES) 
LDFLAGS = -LC:/dev/SDL2/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2

all:
	$(CC) main.cpp $(CFLAGS) $(LDFLAGS) -Ofast -o ray