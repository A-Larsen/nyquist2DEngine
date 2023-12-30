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
#ifndef N2DE_LINES_H_
#define N2DE_LINES_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"

#include "declare.h"
#include "memory.h"

typedef struct _Lines {
    uint8_t count;
    ScreenElement *screenElements;
    uint8_t *update;
    uint8_t update_count;
} Lines;

void lines_init(Lines *lines)
{
    memset(lines, 0, sizeof(Lines));
    lines->screenElements = NULL;
}

void lines_free(Lines *lines)
{
    for(uint8_t i = 0; i < lines->count; ++i) {
        /* SDL_DestroyTexture(lines->screenElements[i].texture); */
        /* SDL_FreeSurface(lines->screenElements[i].surface); */
    }
    free(lines->screenElements);
    free(lines->update);
}

uint8_t lines_create(Lines *lines, SDL_Color *color, uint16_t zIndex)
{
    int i = lines->count++;
    MEMRES(lines->screenElements, sizeof(ScreenElement) * lines->count);
    ScreenElement *screenElements = &lines->screenElements[i];
    lines->screenElements[i].type = ELEMENT_LINE;
    lines->screenElements[i].zIndex = zIndex;
    memcpy(&screenElements->color, color, sizeof(SDL_Color));
    return i;
}

void lines_que(Lines *lines, uint8_t id, SDL_Point *start, SDL_Point *end)
{
    ScreenElement *params = &lines->screenElements[id];
    memcpy(&params->data.line.start, start, sizeof(SDL_Point));
    memcpy(&params->data.line.end, end, sizeof(SDL_Point));

    int j = lines->update_count++;
    MEMRES(lines->update, sizeof(uint8_t) * lines->update_count);
    lines->update[j] = id;
}

void lines_updateQue(Lines *lines, ScreenElement *update_elements[20], uint16_t *count)
{
    for (uint8_t i = 0; i < lines->update_count; ++i) {
        uint8_t id = lines->update[i];
        ScreenElement *screenElement = &lines->screenElements[id];
        update_elements[(*count)] = screenElement;
        memcpy(&update_elements[(*count)]->color, &screenElement->color, sizeof(SDL_Color));
        (*count)++;
    }
}

void lines_reset(Lines *lines)
{
    free(lines->update);
    lines->update = NULL;
    lines->update_count = 0;
}


#endif // N2DE_LINES_H_
