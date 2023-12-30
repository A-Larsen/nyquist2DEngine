/* 
 * Copyright (C) 2022  Austin Larsen
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef N2DE_LUA_LOOPS_H_
#define N2DE_LUA_LOOPS_H_

#include <stdio.h>

#include "begin.h"

int luaLoops_init(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    loops_init(&engine->loops);
    return 0;
}

int luaLoops_add(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int lps = (int)luaL_checknumber(L, 1);
    int id = loops_add(&engine->loops, lps);
    lua_pushnumber(L, id);
    return 1;
}

int luaLoops_loopStart(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    loops_start(&engine->loops, id);
    return 0;
}

int luaLoops_loopEnd(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    loops_end(&engine->loops, id);
    return 0;
}

const struct luaL_Reg luaFunctions_loops[] = {
    {"init", luaLoops_init},
    {"add", luaLoops_add},
    {"loopStart", luaLoops_loopStart},
    {"loopEnd", luaLoops_loopEnd},
    {NULL, NULL}
};

#endif // N2DE_LUA_LOOPS_H_
