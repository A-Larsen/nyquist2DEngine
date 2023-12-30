/* 
 * Copyright (C) 2022  Austin Larsen
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, b,Wut WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef N2DE_LUA_AUDIO_H_
#define N2DE_LUA_AUDIO_H_

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "begin.h"

int luaAudio_init(lua_State *L)
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

int luaAudio_play(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *name = (char *)luaL_checkstring(L, 1);
    int id = audio_getIdByName(&engine->audio, name);
    printf("audio play id: %d\n", id);
    audio_play(&engine->audio, id);
    return 0;
}

int luaAudio_stopAll(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    audio_stopAll(&engine->audio);
    return 0;
}

const struct luaL_Reg luaFunctions_audio[] = {
    {"init", luaAudio_init},
    {"play", luaAudio_play},
    {"stopAll", luaAudio_stopAll},
    {NULL, NULL}
};

#endif // N2DE_LUA_AUDIO_H_
