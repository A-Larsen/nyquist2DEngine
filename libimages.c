#include "./src/lua5.4/lua.h"
#include "./src/lua5.4/lauxlib.h"
#include "./src/N2DE/N2DE.h"

int libimages_create(lua_State *L)
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

    /* lua_topoint(L, 2, &p); */
    int zIndex = (int)luaL_checknumber(L, 2);

    /* if (arg_count == 3) lua_tosize(L, 3, &s); */

    /* SDL_Rect rect = {p.x, p.y, s.w, s.h}; */
    images_create(&engine->images, engine->renderer, path, NULL, zIndex);
    lua_pushnumber(L, engine->images.count - 1);
    return 1;
}

int libimages_rotate(lua_State *L)
{
    int i = (int)luaL_checknumber(L, 1);
    uint32_t rotation = (uint32_t)luaL_checknumber(L, 2);

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    engine->images.screenElements[i].rotation = rotation;
    return 0;
}

int libimages_update(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int id = (int)luaL_checknumber(L, 1);

    SDL_Point p;
    Size s = {0, 0};
    lua_topoint(L, 2, &p);

    if (arg_count == 3) lua_tosize(L, 3, &s);
     
    SDL_Rect r = {p.x, p.y, s.w, s.h};
    images_que(&engine->images, id, &r);
    return 0;
}

int libimages_newGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t i = images_newGroup(&engine->images);
    lua_pushnumber(L, i);
    return 1;
}

int libimages_loadGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    bool sucess = images_loadGroup(&engine->images, id);
    lua_pushboolean(L, sucess);
    return 1;
}

int libimages_saveGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    images_saveGroup(&engine->images, id);
    return 0;
}

int libimages_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    images_quit(&engine->images);
    return 0;
}

luaL_Reg libimages[] = {
    {"create", libimages_create},
    {"rotate", libimages_rotate},
    {"update", libimages_update},
    {"newGroup", libimages_newGroup},
    {"saveGroup", libimages_saveGroup},
    {"loadGroup", libimages_loadGroup},
    {"free", libimages_free},
    {NULL, NULL}
};

int luaopen_extensions_libimages(lua_State *L)
{
    luaL_newlib(L, libimages);
    return 1;
}
