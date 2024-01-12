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
#ifndef N2DE_LUA_MAP_H_
#define N2DE_LUA_MAP_H_

#include <stdio.h>
#include "begin.h"

int lua_MapSet(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int num = (int)luaL_checknumber(L, 1);
    SDL_Point point;
    lua_topoint(L, 2, &point);
    /* int x = (int)luaL_checknumber(L, 1); */
    /* int y = (int)luaL_checknumber(L, 2); */
    engine->worlds[num].map.position.x = point.x;
    engine->worlds[num].map.position.y = point.y;
    
    return 0;
    
}

const struct luaL_Reg luaFunctions_map[] = {
    {"set", lua_MapSet},
    {NULL, NULL}
};

#endif // N2DE_LUA_MAP_H_
