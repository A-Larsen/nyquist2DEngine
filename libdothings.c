#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

__declspec(dllexport) int getWindowWidth(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    printf("window width: %d\n", engine->window_rect.w);
    return 0;
}

__declspec(dllexport) int goodbye(lua_State *L)
{
    printf("goodbye\n");
    return 0;
}

__declspec(dllexport) luaL_Reg mylib[] = {
    {"getWindowWidth", getWindowWidth},
    {"goodbye", goodbye},
    {NULL, NULL}
};

/* __declspec(dllexport) int luaopen_mylib_lib(lua_State *L) */
/* __declspec(dllexport) int luaopen_mylib(lua_State *L) */
__declspec(dllexport) int luaopen_extensions_libdothings(lua_State *L)
{
    luaL_newlib(L, mylib);
    return 1;
}
