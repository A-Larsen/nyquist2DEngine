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
#ifndef N2DE_LUA_TERMINAL_H_
#define N2DE_LUA_TERMINAL_H_

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "begin.h"

int luaTerminal_message(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *str = (char *)luaL_checkstring(L, 1);
    terminal_addMessage(&engine->terminal, str, TERM_MSGNORM);
    return 0;
}

int luaTerminal_close(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    engine->terminal.toggled = false;
    return 0;
}

int luaTerminal_error(lua_State *L) 
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *str = (char *)luaL_checkstring(L, 1);
    terminal_addMessage(&engine->terminal, str, TERM_MSGERROR);
    return 0;
}

const struct luaL_Reg luaFunctions_terminal[] = {
    {"message", luaTerminal_message},
    {"error", luaTerminal_error},
    {"close", luaTerminal_close},
    {NULL, NULL}
};

#endif // N2DE_LUA_TERMINAL_H_
