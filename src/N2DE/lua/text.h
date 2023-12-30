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
#ifndef N2DE_LUA_TEXT_H_
#define N2DE_LUA_TEXT_H_

#include <stdio.h>

#include "begin.h"

int luaText_create(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    SDL_Color rgba_foreground;
    SDL_Color rgba_background;

    const char *file = luaL_checkstring(L, 1);
    char path[255];
    memset(path, 0, 255);
    sprintf(path, RESPATH "/fonts/%s", file);
    lua_Number fontSize = luaL_checknumber(L, 2);
    const char *text = luaL_checkstring(L, 3);
    int zIndex = (int)luaL_checknumber(L, 4);

    lua_tocolor(L, 5, &rgba_foreground);
    lua_tocolor(L, 6, &rgba_background);
    if (rgba_background.a == 0) rgba_background.a = 1;

    int i = fonts_create(&engine->fonts, path, fontSize, engine->renderer,
                         text, zIndex, rgba_foreground, rgba_background);

    lua_pushnumber(L, i);
    return 1;
}

int luaText_change(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    SDL_Color rgba_foreground;
    SDL_Color rgba_background;

    int id = (int)luaL_checknumber(L, 1);
    const char *text = luaL_checkstring(L, 2);

    lua_tocolor(L, 3, &rgba_foreground);
    lua_tocolor(L, 4, &rgba_background);
    if (rgba_background.a == 0) rgba_background.a = 1;

    fonts_change(&engine->fonts, id, engine->renderer, 
                 text, rgba_foreground, rgba_background);
    lua_pushnumber(L, engine->fonts.count - 1);
    return 1;
}

int luaText_update(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;

    int id = (int)luaL_checknumber(L, 1);
    lua_topoint(L, 2, &p);

    fonts_que(&engine->fonts, id, p);
    return 0;
}

int luaText_getSize(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    int i = (int)luaL_checknumber(L, 1);

    if (engine->fonts.screenElements == NULL) {
        lua_pushnil(L);
        return 1;
    }
    lua_newtable(L);
    lua_pushstring(L, "w");
    lua_pushnumber(L, engine->fonts.screenElements[i].position.w);
    lua_settable(L, -3);
    lua_pushstring(L, "h");
    lua_pushnumber(L, engine->fonts.screenElements[i].position.h);
    lua_settable(L, -3);
    return 1;
}

int luaText_clear(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    fonts_clear(&engine->fonts);
}

const struct luaL_Reg luaFunctions_text[] = {
    {"create", luaText_create},
    {"update", luaText_update},
    {"getSize", luaText_getSize},
    {"change", luaText_change},
    {"clear", luaText_clear},
    {NULL, NULL}
};

#endif // N2DE_LUA_TEXT_H_
