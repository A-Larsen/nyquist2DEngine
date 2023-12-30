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
#ifndef N2DE_LUA_RECTS_H_
#define N2DE_LUA_RECTS_H_

#include <stdio.h>

#include "begin.h"

int luaRects_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Color c;
    lua_tocolor(L, 1, &c);
    bool filled = (bool)lua_toboolean(L, 2);
    uint16_t zIndex = (uint16_t)lua_tonumber(L, 3);
    uint8_t id = rects_create(&c, filled, zIndex);
    lua_pushnumber(L, id);
    return 1;
}

int luaRects_update(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;
    Size s;
    uint8_t id  = (uint8_t)luaL_checknumber(L, 1);
    lua_topoint(L, 2, &p);
    lua_tosize(L, 3, &s);
    SDL_Rect r = {
        p.x, p.y, s.w, s.h
    };
    rects_que(id, &r);
    return 0;
}

int luaRects_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    rects_free();
    return 0;
}

const struct luaL_Reg luaFunctions_rects[] = {
    {"create", luaRects_create},
    {"update", luaRects_update},
    {"free", luaRects_free},
    {NULL, NULL}
};

#endif // N2DE_LUA_RECTS_H_
