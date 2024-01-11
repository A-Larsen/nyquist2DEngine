ENGINE = n2de
LIBS = ./libs/lua54.lib ./libs/SDL2.lib ./libs/SDL2_image.lib ./libs/SDL2_ttf.lib ./libs/sqlite3.lib
CFLAGS = -L ./libs -Wextra -Wall
DEFINES = -D _WIN32
DEBUGFLAGS = -g
CC = x86_64-w64-mingw32-gcc
