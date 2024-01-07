#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

int libloops_init(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    loops_init(&engine->loops);
    return 0;
}

int libloops_add(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int lps = (int)luaL_checknumber(L, 1);
    int id = loops_add(&engine->loops, lps);
    lua_pushnumber(L, id);
    return 1;
}

int libloops_loopStart(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    loops_start(&engine->loops, id);
    return 0;
}

int libloops_loopEnd(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    loops_end(&engine->loops, id);
    return 0;
}

luaL_Reg libloops[] = {
    {"init", libloops_init},
    {"add", libloops_add},
    {"loopStart", libloops_loopStart},
    {"loopEnd", libloops_loopEnd},
    {NULL, NULL}
};

int luaopen_extensions_libloops(lua_State *L)
{
    luaL_newlib(L, libloops);
    return 1;
}
