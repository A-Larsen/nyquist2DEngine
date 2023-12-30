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
#ifndef N2DE_LUA_FORMAT_H
#define N2DE_LUA_FORMAT_H

#include <stdio.h>

#include "begin.h"

int luaFormats_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *text = (char *)luaL_checkstring(L, 1);


    /* int i = fonts_create(&engine->format_strings, path, fontSize, engine->renderer, */
    /*                      text, zIndex, rgba_foreground, rgba_background); */
    /* int i = fonts_create(&engine->format_strings, path, fontSize, engine->renderer, */
    /*                      text, zIndex, rgba_foreground, rgba_background); */

    uint8_t i = formats_add(text);
    lua_pushnumber(L, i);
    return 1;
}


int luaFormats_clear(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    formats_clear();
}

int luaFormats_newGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    /* uint16_t i = fonts_newGroup(&engine->format_strings); */
    uint8_t i = formats_newGroup();
    lua_pushnumber(L, i);
    return 1;
}

int luaFormats_loadGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    bool success = formats_loadGroup(id);
    lua_pushboolean(L, success);
    return 1;
}

int luaFormats_saveGroup(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    formats_saveGroup(id);
    return 0;
}

int luaFormats_getString(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    /* fonts_saveGroup(&engine->format_strings, id); */
    char string[FORMATS_MAX_STRING];
    bool success  = formats_getString(string, id);
    if (success) {
        lua_pushstring(L, string);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

const struct luaL_Reg luaFunctions_formats[] = {
    {"create", luaFormats_create},
    {"clear", luaFormats_clear},
    {"getString", luaFormats_getString},
    {"newGroup", luaFormats_newGroup},
    {"saveGroup", luaFormats_saveGroup},
    {"loadGroup", luaFormats_loadGroup},
    {NULL, NULL}
};

#endif // N2DE_LUA_FORMAT_H
