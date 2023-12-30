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
#ifndef N2DE_LUA_IMAGE_H_
#define N2DE_LUA_IMAGE_H_

#include <stdint.h>
#include <stdio.h>

#include "begin.h"

int luaImage_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    SDL_Point p;
    Size s = {.w  = 0, .h = 0};
    int arg_count = lua_gettop(L);

    const char *file = luaL_checkstring(L, 1);
    char path[255];
    memset(path, 0, 255);
    sprintf(path, RESPATH "/images/%s", file);

    /* lua_topoint(L, 2, &p); */
    int zIndex = (int)luaL_checknumber(L, 2);

    /* if (arg_count == 3) lua_tosize(L, 3, &s); */

    /* SDL_Rect rect = {p.x, p.y, s.w, s.h}; */
    images_create(&engine->images, engine->renderer, path, NULL, zIndex);
    lua_pushnumber(L, engine->images.count - 1);
    return 1;
}

int luaImage_rotate(lua_State *L)
{
    int i = (int)luaL_checknumber(L, 1);
    uint32_t rotation = (uint32_t)luaL_checknumber(L, 2);

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    engine->images.screenElements[i].rotation = rotation;
    return 0;
}

int luaImage_update(lua_State *L)
{
    int arg_count = lua_gettop(L);
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int id = (int)luaL_checknumber(L, 1);

    SDL_Point p;
    Size s = {0, 0};
    lua_topoint(L, 2, &p);

    if (arg_count == 3) lua_tosize(L, 3, &s);
     
    SDL_Rect r = {p.x, p.y, s.w, s.h};
    images_que(&engine->images, id, &r);
    return 0;
}

int luaImage_newGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t i = images_newGroup(&engine->images);
    lua_pushnumber(L, i);
    return 1;
}

int luaImages_loadGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    bool sucess = images_loadGroup(&engine->images, id);
    lua_pushboolean(L, sucess);
    return 1;
}

int luaImages_saveGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    images_saveGroup(&engine->images, id);
    return 0;
}

int luaImages_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    images_quit(&engine->images);
    return 0;
}

const struct luaL_Reg luaFunctions_images[] = {
    {"create", luaImage_create},
    {"rotate", luaImage_rotate},
    {"update", luaImage_update},
    {"newGroup", luaImage_newGroup},
    {"saveGroup", luaImages_saveGroup},
    {"loadGroup", luaImages_loadGroup},
    {"free", luaImages_free},
    {NULL, NULL}
};

#endif // N2DE_LUA_IMAGE_H_
