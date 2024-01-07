#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

__declspec(dllexport) int libformats_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *text = (char *)luaL_checkstring(L, 1);
    uint8_t i = formats_add(text);
    lua_pushnumber(L, i);
    return 1;
}

__declspec(dllexport) int libformats_clear(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    formats_clear();
}

__declspec(dllexport) int libformats_newGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint8_t i = formats_newGroup();
    lua_pushnumber(L, i);
    return 1;
}

__declspec(dllexport) int libformats_loadGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    bool success = formats_loadGroup(id);
    lua_pushboolean(L, success);
    return 1;
}

__declspec(dllexport) int libformats_saveGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    formats_saveGroup(id);
    return 0;
}

__declspec(dllexport) int libformats_getString(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    /* fonts_saveGroup(&engine->format_strings, id); */
    char string[FORMATS_MAX_STRING];
    bool success  = formats_getString(string, id);
    if (success) {
        lua_pushstring(L, string);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

__declspec(dllexport) luaL_Reg libformats[] = {
    {"create", libformats_create},
    {"clear", libformats_clear},
    {"getString", libformats_getString},
    {"newGroup", libformats_newGroup},
    {"saveGroup", libformats_saveGroup},
    {"loadGroup", libformats_loadGroup},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_extensions_libformats(lua_State *L)
{
    luaL_newlib(L, libformats);
    return 1;
}
