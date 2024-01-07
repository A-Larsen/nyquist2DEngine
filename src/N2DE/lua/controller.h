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
#ifndef _N2DE_CONTROLLER_H_
#define _N2DE_CONTROLLER_H_

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "begin.h"

int luaController_assign(lua_State *L)
{

    return 1;
}

const struct luaL_Reg luaFunctions_controller[] = {
    {"assign", luaController_assign},
    {NULL, NULL}
};

#endif // _N2DE_CONTROLLER_H_
