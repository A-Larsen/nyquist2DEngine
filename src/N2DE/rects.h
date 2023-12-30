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
#ifndef N2DE_RECTS_H_
#define N2DE_RECTS_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"

#include "declare.h"
#include "memory.h"
#include "shapes.h"

static uint8_t count;
static ScreenElement *screenElements;
static uint8_t *update;
static uint8_t update_count;

void rects_init()
{
    update_count = 0;
    screenElements = NULL;
}

void rects_free()
{
    /* for(uint8_t i = 0; i < count; ++i) { */
    /*     screenElements_free(&screenElements[i]); */
    /* } */
    free(screenElements);
    screenElements = NULL;
    count = 0;
}

uint8_t rects_create(SDL_Color *color, bool filled, uint16_t zIndex)
{
    int i = count++;
    MEMRES(screenElements, sizeof(ScreenElement) * count);
    ScreenElement *params = &screenElements[i];
    params->zIndex = zIndex;
    screenElements[i].type = ELEMENT_RECT;
    memcpy(&params->color, color, sizeof(SDL_Color));
    params->filled = filled;
    return i;
}

void rects_que(uint8_t id, SDL_Rect *r)
{
    ScreenElement *params = &screenElements[id];
    memcpy(&params->position, r, sizeof(SDL_Rect));

    int j = update_count++;
    MEMRES(update, sizeof(uint8_t) * update_count);
    update[j] = id;
}

void rects_updateQue(ScreenElement *update_elements[20], uint16_t *count)
{
    for (uint8_t i = 0; i < update_count; ++i) {
        uint8_t id = update[i];
        ScreenElement *screenElement = &screenElements[id];
        update_elements[(*count)] = screenElement;
        /* memcpy(&update_elements[(*count)]->color, &screenElement->color, sizeof(SDL_Color)); */
        (*count)++;
    }
}

void rects_reset()
{
    free(update);
    update = NULL;
    update_count = 0;
}

#endif // N2DE_RECTS_H_
