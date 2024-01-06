#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

__declspec(dllexport) int init(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int i = 0;
    if (lua_istable(L, 1)) {
        lua_pushnil(L);
        while(lua_next(L, 1) != 0) {
           i++;
           lua_pop(L, 1);
        }
    }
    int size = i;

    i = 0;
    /* WavFile wav_files[size]; */
    WavFile *wav_files = NULL;
    MEMRES(wav_files, sizeof(WavFile) * size);

    if (lua_istable(L, 1)) {
        lua_pushnil(L);
        while(lua_next(L, 1) != 0) {
            const char *name  = luaL_checkstring(L, -2);

            lua_pushnil(L);
            int j = 0;
            while(lua_next(L, -2) != 0) {
                const char *type  = luaL_checkstring(L, -2);
                if (strcmp(type, "file") == 0) {
                    const char *file  = luaL_checkstring(L, -1);
                    int len = strlen(name);
                    memcpy(wav_files[i].name, name, len * sizeof(char));
                    wav_files[i].name[len] = '\0';
                    char path[500];
                    sprintf(path, RESPATH "/wavs/%s",  file);
                    len = strlen(path);
                    memcpy(wav_files[i].path, path, len  * sizeof(char));
                    wav_files[i].path[len] = '\0';
                }
                if (strcmp(type, "loop") == 0) {
                    int loop  = (int)luaL_checknumber(L, -1);
                    wav_files[i].loop = loop;

                }
                j++;
                lua_pop(L, 1);
            }
            i++;
            lua_pop(L, 1);
        }
    }
    audio_init(&engine->audio, wav_files, size);
    free(wav_files);
    return 0;
}

__declspec(dllexport) int play(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *name = (char *)luaL_checkstring(L, 1);
    int id = audio_getIdByName(&engine->audio, name);
    printf("audio play id: %d\n", id);
    audio_play(&engine->audio, id);
    return 0;
}

__declspec(dllexport) int stopAll(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    audio_stopAll(&engine->audio);
    return 0;
}

__declspec(dllexport) int quit(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    audio_quit(&engine->audio);
    return 0;
}

__declspec(dllexport) luaL_Reg libaudio[] = {
    {"init", init},
    {"play", play},
    {"stopAll", stopAll},
    {"quit", quit},
    {NULL, NULL}
};

/* __declspec(dllexport) int luaopen_mylib_lib(lua_State *L) */
/* __declspec(dllexport) int luaopen_mylib(lua_State *L) */
__declspec(dllexport) int luaopen_extensions_libaudio(lua_State *L)
{
    luaL_newlib(L, libaudio);
    return 1;
}
