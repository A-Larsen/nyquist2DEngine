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
#ifndef N2DE_CIRCLES_H
#define N2DE_CIRCLES_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"

#include "declare.h"
#include "memory.h"
#include "shapes.h"
#include "memory.h"

typedef struct _CircleParams {
    SDL_Point center; 
    uint16_t radius; 
    SDL_Point tilt;
    SDL_Color color;
} CircleParams;

typedef struct _Circles {
    uint8_t count;
    CircleParams *params;
} Circles;

void circles_init(Circles *circles)
{
    memset(circles, 0, sizeof(Circles));
    circles->params = NULL;
}

void circles_free(Circles *circles)
{
    free(circles->params);
    circles->params = NULL;
    circles->count = 0;
}

void circles_create(Circles *circles, SDL_Color *color, SDL_Point *center,
                  uint16_t radius, SDL_Point *tilt)
{

    int i = circles->count++;

    MEMRES(circles->params, sizeof(CircleParams) *
                                   circles->count);

    CircleParams *params = &circles->params[i];
    params->radius = radius;
    memcpy(&params->center, center, sizeof(SDL_Point));
    memcpy(&params->tilt, tilt, sizeof(SDL_Point));
    memcpy(&params->color, color, sizeof(SDL_Color));
}


void circles_update(Circles *circles, SDL_Renderer *renderer)
{
    for (uint8_t i = 0; i < circles->count; ++i) {
        CircleParams *params = &circles->params[i];
        shapes_drawCircle(renderer, params->color, params->center,
                          params->radius, params->tilt);
    }
    circles_free(circles);
}

#endif // N2DE_CIRCLES_H
