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
#ifndef N2DE_INPUTS_H_
#define N2DE_INPUTS_H_

#include "memory.h"
#include <stdint.h>
#include <string.h>
#include "../SDL2/SDL.h"
#include "declare.h"

typedef struct _InputInfo {
    int controller_id;
    SDL_GameController *controller;
    ControlsKV keyboard_controls[20];
    ControlsKV gamepad_controls[20];
    int keyboard_controls_length;
    int gamepad_controls_length;
} InputInfo;

typedef struct _Inputs {
    InputInfo inputInfo[4];
    uint8_t count;
} Inputs;

#endif // N2DE_INPUTS_H_
