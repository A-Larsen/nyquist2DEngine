#ifndef N2DE_LUA_BEGIN_H
#define N2DE_LUA_BEGIN_H

#include <stdbool.h>

#include "../../lua5.4/lua.h"
#include "../../lua5.4/lauxlib.h"
#include "../../lua5.4/lualib.h"
#include "../../SDL2/SDL.h"
#include "../../SDL2/SDL_ttf.h"

#include "../fonts.h"
#include "../world.h"
#include "../memory.h"
#include "../images.h"
#include "../database.h"
#include "../config.h"
#include "../sprites.h"
#include "../shapes.h"
#include "../loops.h"
#include "../audio.h"
#include "../circles.h"
#include "../rects.h"
#include "../declare.h"
#ifndef PRODUCTION
#include "../terminal.h"
#endif
#include "../rects.h"
#include "../lines.h"
#include "../inputs.h"
#include "../players.h"


#define LUA_GETENGINE(L, engine)\
    lua_getglobal(L, "nyq"); \
    lua_getfield(L, -1, "_cdata"); \
    engine = (Nyquist2DEngine *)lua_touserdata(L, -1); \
    lua_pop(L, 2);

/* typedef struct _Player { */
/*     SDL_Point position; */
/* } Player; */

typedef struct _Nyquist2DEngine {
    SDL_Rect window_rect;
    int window_scale;
    SDL_Window* window;
    Fonts format_strings;
    Fonts fonts;
    /* World world; */
    World worlds[5];
    Images images;
    /* Player player; */
    SDL_Renderer *renderer;
    /* ControlsKV controls[20]; */
    /* int controls_length; */
    lua_State *lua_state;
    SDL_Event event;
    Database database;
    char lua_path[50];
    bool quit;
    double deltaTime;
    bool updateCircle;
    bool updateRect;
    Sprites sprites;
    Loops loops;
    Audio audio;
    Circles circles;
#ifndef PRODUCTION
    Terminal terminal;
#endif
    KeyInfo keyinfo;
    bool running;
    uint8_t frame_rate;
    bool initialized;
    Lines lines;
    ScreenElement *update_elements[MAX_UPDATE_ELEMENTS];
    bool hasFrameCap;
    /* uint8_t player_control_type_flags; */
    /* uint8_t controller_ids[4]; */
    /* SDL_GameController *controllers[4]; */
    /* uint8_t player_count; */
    Inputs inputs;
    Players players;
    /* UpdateElement update_elements[20]; */
} Nyquist2DEngine;

SchemaKV *lua_findschemaKV(Schema *p, char *name)
{
    while(p->kv) {
       if(!strcmp(p->name, name)) 
           return p->kv;
       p++;
    }
    return NULL;
}

void lua_topoint(lua_State *L, int index, SDL_Point *p)
{
    if (lua_istable(L, index)) {
        lua_getfield(L, index, "x");
        p->x = (int)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, index, "y");
        p->y = (int)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
}

void lua_tosize(lua_State *L, int index, Size *s)
{
    if (lua_istable(L, index)) {
        lua_getfield(L, index, "w");
        s->w = (int)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, index, "h");
        s->h = (int)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
}

void lua_tocolor(lua_State *L, int index, SDL_Color *rgba)
{
    if (lua_istable(L , index)) {
        lua_getfield(L, index, "r");
        rgba->r = (int)luaL_checknumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, index, "g");
        rgba->g = (int)luaL_checknumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, index, "b");
        rgba->b = (int)luaL_checknumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, index, "a");
        rgba->a = (int)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }
}

void lua_freeSchema(lua_State *L)
{
    lua_getglobal(L, "SCHEMA");
    Schema *schema = (Schema *)lua_touserdata(L, -1);
    if (schema) {
        Schema *p = schema;
        while(p->kv) {
            if (p->kv) {
                free(p->kv);
            }
            p++;
        }
    }
    free(schema);
}

#endif // N2DE_LUA_BEGIN_H
