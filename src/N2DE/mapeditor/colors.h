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
#ifndef N2DE_MAPEDITOR_COLORS_H_
#define N2DE_MAPEDITOR_COLORS_H_
#include "../declare.h"

#define COLOR_BLACK 0
#define COLOR_GREY 1

SDL_Color mapeditor_colors[] = {
    [COLOR_BLACK] = {.r = 0, .g = 0, .b = 0, .a = 0},
    [COLOR_GREY] = {.r = 230, .g = 230, .b = 230, .a = 0}
};

#endif // N2DE_MAPEDITOR_H_
