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
#ifndef N2DE_LUA_WAITING_H_
#define N2DE_LUA_WAITING_H_

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "begin.h"

int luaWaiting_new(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint64_t time = (uint64_t)luaL_checknumber(L, 1);
    /* char *uuid = waiting_new(&engine->waiting, time); */
    /* lua_pushstring(L, uuid); */
    return 1;
}

int luaWaiting_isFinished(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    bool isString = lua_isstring(L, 1);

    if (isString) {
        char *uuid  = (char *)luaL_checkstring(L, 1);
        /* bool isFinished = waiting_finished(&engine->waiting, uuid); */
        /* lua_pushboolean(L, isFinished); */
        return 1;
    }

    lua_pushboolean(L, true);
    return 1;
}

const struct luaL_Reg luaFunctions_waiting[] = {
    {"new", luaWaiting_new},
    {"isFinished", luaWaiting_isFinished},
    {NULL, NULL}
};

#endif // N2DE_LUA_WAITING_H_
