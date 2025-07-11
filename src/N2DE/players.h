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
#ifndef N2DE_PLAYERS_H_
#define N2DE_PLAYERS_H_

#include "memory.h"
#include <stdint.h>
#include <string.h>
#include "../SDL2/SDL.h"
#include "declare.h"

typedef struct _PlayerInfo {
    SDL_Point position;
} PlayerInfo;

typedef struct _Players {
    PlayerInfo playerInfo[4];
    uint8_t count;
} Players;


#endif // N2DE_PLAYERS_H_
