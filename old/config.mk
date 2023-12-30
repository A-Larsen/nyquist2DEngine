OUT = n2de

# LIBS =   -lm -llua5.3 -lSDL2 -lSDL2_image -lSDL2_ttf -lsqlite3 -luuid -lSDL2_mixer
LIBS =   -lm -llua5.3 -lSDL2 -lSDL2_image -lSDL2_ttf -lsqlite3 -luuid
# WINLIBS = .\libs\lua53.lib .\libs\SDL2.lib .\libs\SDL2_image.lib .\libs\SDL2_ttf.lib .\libs\sqlite3.lib .\libs\SDL2_mixer.lib
WINLIBS = .\libs\lua53.lib .\libs\SDL2.lib .\libs\SDL2_image.lib .\libs\SDL2_ttf.lib .\libs\sqlite3.lib

CFLAGS = -L ./libs -Wextra -Wall
WINCFLAGS = /EHsc

DEFINES =
WINDEFINES = 

DEBUGFLAGS = -g
WINDEBUGFLAGS = /Zi

CC = gcc
WINCC = cl
