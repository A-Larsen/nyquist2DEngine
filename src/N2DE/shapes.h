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
#ifndef N2DE_SHAPES_H_
#define N2DE_SHAPES_H_

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../SDL2/SDL.h"

#define TAU (M_PI * 2.0f)


void shapes_drawCircle(SDL_Renderer* renderer, SDL_Color color, 
                       SDL_Point center, int radius, SDL_Point tilt)
{
    float accuracy = 0.2f;
    float x1, x2, y1, y2 = 0;
    radius -= 1;
    
    for(float i = 0; i <= TAU + accuracy; i += accuracy) {

        x1 = x2, y1 = y2;

        x2 = sinf(i + accuracy * tilt.x);
        y2 = cosf(i + accuracy * tilt.y);

        if (!(i > 0)) continue;

        int x_1 = x1 * radius + center.x;
        int y_1  = y1 * radius + center.y;
        int x_2 = x2 * radius + center.x;
        int y_2  = y2 * radius + center.y;

        // drawing border
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x_1, y_1, x_2, y_2);
    }
} // drawCircle

// TODO:
// draw a filled circle with the standard circle formula
void shapes_fillCircle()
{

}

#endif // N2DE_SHAPES_H_
