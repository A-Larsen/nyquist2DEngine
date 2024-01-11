#include "./src/lua5.4/lua.h"
#include "./src/lua5.4/lauxlib.h"
#include "./src/N2DE/N2DE.h"

int libsprites_new(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;
    Size s = {.w  = 0, .h = 0};

    int arg_count = lua_gettop(L);
    const char *file = luaL_checkstring(L, 1);
    char path[255];
    memset(path, 0, 255);
    sprintf(path, RESPATH "/images/%s", file);
    SDL_Rect rect = {p.x, p.y, s.w, s.h};
    int i = sprites_new(&engine->sprites, engine->renderer, path);
    lua_pushnumber(L, i);
    return 1;
}
int libsprites_addCrops(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    int zIndex = (int)luaL_checknumber(L, 3);

    if (!lua_istable(L, 4)) {
        N2DE_ERROR("is not table");
        exit(1);
    }
    int size = 0;

    lua_pushnil(L);
    while (lua_next(L, 4)) {
        lua_pop(L, 1);
        size++;
    }


    if (size <= 0) {
        return 0;
    }
    /* SDL_Rect rects[csize]; */
    SDL_Rect *rects = NULL;
    MEMRES(rects, sizeof(SDL_Rect) * size);

    int i = 0;
    lua_pushnil(L);
    while (lua_next(L, 4)) {
        // tables within tables have their own indexes
        /* int idx = (int)lua_tonumber(L, 4); */

        lua_pushnil(L);
        while(lua_next(L, 6)) {
            char *key  = (char *)luaL_checkstring(L, -2);
            int val  = (int)luaL_checknumber(L, -1);
            if (!strcmp(key, "x")) {
                rects[i].x = val;
            }
            if (!strcmp(key, "y")) {
                rects[i].y = val;
            }
            if (!strcmp(key, "w")) {
                rects[i].w = val;
            }
            if (!strcmp(key, "h")) {
                rects[i].h = val;
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
        i++;
    }
    sprites_addCrops(&engine->sprites, engine->renderer, id, type, rects, size, zIndex);
    free(rects);

    return 0;
}


int libsprites_updateCrop(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    int frame_number = (int)luaL_checknumber(L, 3) - 1;
    SDL_Point p;
    lua_topoint(L, 4, &p);
    Size size = {0, 0};
    Size scale = {1, 1};
    
    if (lua_istable(L, 5)) {
        lua_pushnil(L);
        while(lua_next(L, 5)) {
            char *field = (char *)luaL_checkstring(L, -2);
            if (strcmp(field, "size") == 0) {
                /* printf("size"); */
                int type = 0;
                type = lua_getfield(L, -1, "w");
                size.w = (int)luaL_checknumber(L, -1);
                lua_pop(L, 1);

                type = lua_getfield(L, -1, "h");
                size.h = (int)luaL_checknumber(L, -1);
                lua_pop(L, 1);
            }
            if (strcmp(field, "scale") == 0) {
                int type = 0;
                type = lua_getfield(L, -1, "w");
                scale.w = (int)luaL_checknumber(L, -1);
                lua_pop(L, 1);

                type = lua_getfield(L, -1, "h");
                scale.h = (int)luaL_checknumber(L, -1);
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }

    }

    sprites_updateCrop(&engine->sprites, engine->renderer, p, size, scale,
                   frame_number, id, type);
    return 0;
}


int libsprites_rotateCrop(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    uint32_t rotation = (uint32_t)luaL_checknumber(L, 2);
    sprites_rotateCrop(&engine->sprites, id, rotation);
    return 0;
}


int libsprites_getCurrentFrame(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    Crops *crop = sprites_findCrops(&engine->sprites, id, type);

    int frame_number = crop->current_frame;
    lua_pushnumber(L, frame_number);
    return 1;
}


int libsprites_animate(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    int speed = (int)luaL_checknumber(L, 3);
    SDL_Point p;
    lua_topoint(L, 4, &p);
    Size s = {0, 0};

    if (arg_count == 5) {
        lua_tosize(L, 5, &s);
    }

    int frame_numer = \
        sprites_animate(&engine->sprites, engine->renderer, p, s,
                        engine->deltaTime, speed, id, type);
    lua_pushnumber(L, frame_numer);
    return 1;
}


int libsprites_countCrops(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    Crops *crop = sprites_findCrops(&engine->sprites, id, type);
    lua_pushnumber(L, crop->count);
    return 1;
}


int libsprites_quit(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    sprites_free(&engine->sprites);
    return 0;
}


int libsprites_getRect(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (int)luaL_checknumber(L, 1);
    char *type = (char *)luaL_checkstring(L, 2);
    uint16_t frame = (int)luaL_checknumber(L, 3);
    SDL_Rect rect;
    sprites_getRect(&rect, &engine->sprites, id, type, frame);
    printf("w: %d, h: %d\n", rect.w, rect.h);

    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, rect.x);
    lua_settable(L, -3);

    lua_pushstring(L, "y");
    lua_pushnumber(L, rect.y);
    lua_settable(L, -3);

    lua_pushstring(L, "w");
    lua_pushnumber(L, rect.w);
    lua_settable(L, -3);

    lua_pushstring(L, "h");
    lua_pushnumber(L, rect.h);
    lua_settable(L, -3);

    return 1;
}

luaL_Reg libsprites[] = {
    {"new", libsprites_new},
    {"addCrops", libsprites_addCrops},
    {"updateCrop", libsprites_updateCrop},
    {"rotateCrop", libsprites_rotateCrop},
    {"getCurrentFrame", libsprites_getCurrentFrame},
    {"countCrops", libsprites_countCrops},
    {"animate", libsprites_animate},
    {"quit", libsprites_quit},
    {"getRect", libsprites_getRect},
    {NULL, NULL}
};

int luaopen_extensions_libsprites(lua_State *L)
{
    luaL_newlib(L, libsprites);
    return 1;
}
