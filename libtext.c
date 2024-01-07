#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

int libtext_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    SDL_Color rgba_foreground;
    SDL_Color rgba_background;

    const char *file = luaL_checkstring(L, 1);
    char path[255];
    memset(path, 0, 255);
    sprintf(path, RESPATH "/fonts/%s", file);
    lua_Number fontSize = luaL_checknumber(L, 2);
    const char *text = luaL_checkstring(L, 3);
    int zIndex = (int)luaL_checknumber(L, 4);

    lua_tocolor(L, 5, &rgba_foreground);
    lua_tocolor(L, 6, &rgba_background);
    if (rgba_background.a <= 0) rgba_background.a = 1;

    int i = fonts_create(&engine->fonts, path, fontSize, engine->renderer,
                         text, zIndex, rgba_foreground, rgba_background);

    lua_pushnumber(L, i);
    return 1;
}

int libtext_change(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    SDL_Color rgba_foreground;
    SDL_Color rgba_background;

    int id = (int)luaL_checknumber(L, 1);
    const char *text = luaL_checkstring(L, 2);

    lua_tocolor(L, 3, &rgba_foreground);
    lua_tocolor(L, 4, &rgba_background);
    if (rgba_background.a <= 0) rgba_background.a = 1;

    fonts_change(&engine->fonts, id, engine->renderer, 
                 text, rgba_foreground, rgba_background);
    lua_pushnumber(L, engine->fonts.count - 1);
    return 1;
}

int libtext_update(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;

    int id = (int)luaL_checknumber(L, 1);
    lua_topoint(L, 2, &p);

    fonts_que(&engine->fonts, id, p);
    return 0;
}

int libtext_getSize(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int i = (int)luaL_checknumber(L, 1);

    if (engine->fonts.screenElements == NULL) {
        lua_pushnil(L);
        return 1;
    }
    lua_newtable(L);
    lua_pushstring(L, "w");
    lua_pushnumber(L, engine->fonts.screenElements[i].position.w);
    lua_settable(L, -3);
    lua_pushstring(L, "h");
    lua_pushnumber(L, engine->fonts.screenElements[i].position.h);
    lua_settable(L, -3);
    return 1;
}

int libtext_clear(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    fonts_clear(&engine->fonts);
}

luaL_Reg libtext[] = {
    {"create", libtext_create},
    {"update", libtext_update},
    {"getSize", libtext_getSize},
    {"change", libtext_change},
    {"clear", libtext_clear},
    {NULL, NULL}
};

int luaopen_extensions_libtext(lua_State *L)
{
    luaL_newlib(L, libtext);
    return 1;
}
