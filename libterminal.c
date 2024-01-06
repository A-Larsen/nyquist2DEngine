#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

__declspec(dllexport) int libterminal_message(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *str = (char *)luaL_checkstring(L, 1);
    terminal_addMessage(&engine->terminal, str, TERM_MSGNORM);
    return 0;
}

__declspec(dllexport) int libterminal_close(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    engine->terminal.toggled = false;
    return 0;
}

__declspec(dllexport) int libterminal_error(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *str = (char *)luaL_checkstring(L, 1);
    terminal_addMessage(&engine->terminal, str, TERM_MSGERROR);
    return 0;
}

__declspec(dllexport) int libterminal_quit(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    terminal_free(&engine->terminal);
    return 0;
}


__declspec(dllexport) luaL_Reg libterminal[] = {
    {"message", libterminal_message},
    {"error", libterminal_error},
    {"termclose", libterminal_close},
    {"quit", libterminal_quit},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_extensions_libterminal(lua_State *L)
{
    luaL_newlib(L, libterminal);
    return 1;
}
