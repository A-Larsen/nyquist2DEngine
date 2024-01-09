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
#ifndef N2DE_DECLARE_H_
#define N2DE_DECLARE_H_

#include <stdbool.h>
#include <stdio.h>

#include "../SDL2/SDL.h"
#include "error.h"
#define MAX_UPDATE_ELEMENTS 800
#define ELEMENT_STRING_SIZE 1024
#define MAX_ZINDEX 20

typedef enum _SC_TYPES {
    ELEMENT_FONT,
    ELEMENT_IMAGE,
    ELEMENT_SPRITE,
    ELEMENT_RECT,
    ELEMENT_LINE,
    ELEMENT_SIZE
} SC_TYPES;


/* #if defined(_WIN32) */
/* #pragma pack(push, 1) */
/* typedef struct _ScreenElement */
/* #elif defined(__GNUC__) */
/* typedef struct __attribute__((__packed__)) _ScreenElement */
/* #endif */

typedef struct _ScreenElement
{
    SDL_Texture *texture;
    SDL_Surface *surface;
    SDL_Rect position;
    uint32_t rotation : 10; // degrees, 0 - 1023 only
    bool filled : 1;
    union {
        char string[ELEMENT_STRING_SIZE];
        SDL_Rect rect;
        struct {
            SDL_Point start;
            SDL_Point end;
        } line;
        struct  {
            uint32_t radius;
            SDL_Point center;
        } circle;

    }data;
    uint8_t zIndex : 4;
    uint8_t type : 3;
    SDL_Color color;
} ScreenElement;

/* #if defined(_WIN32) */
/* #pragma pack(pop) */
/* #endif */


typedef struct _Size {
    int w;
    int h;
} Size;

typedef struct _ControlsKV {
    char key[50];
    char alias[50];
    bool isPressed : 1;
    bool triggered : 1;
    bool keyTriggered : 1;
    bool repeat : 1;
    bool previously_pressed: 1;
} ControlsKV ;

typedef struct _KeyInfo {
    uint8_t * state;
    SDL_Keycode key;
    bool isDown;
} KeyInfo;

void screenElements_free(ScreenElement *screenElement)
{
    SDL_DestroyTexture(screenElement->texture);
    SDL_FreeSurface(screenElement->surface);
}

void screenElements_update(ScreenElement **update_elements, uint16_t update_elements_count, SDL_Renderer *renderer, SDL_Rect *window)
{
    /* uint16_t skip_count = 0; */
    for (int i = 0; i < MAX_ZINDEX; ++i) {
        for (int j = 0; j < update_elements_count; ++j) {
            SDL_Texture *texture = update_elements[j]->texture;
            SDL_Rect *rect = &update_elements[j]->position;
            uint32_t rotation = update_elements[j]->rotation;
            uint8_t zIndex = update_elements[j]->zIndex;
            SDL_Rect result;
            bool intersect  = SDL_IntersectRect(rect, window, &result);
            if (!intersect) {
                /* skip_count++; */
                continue;
            };
            if (zIndex == i) {
                switch (update_elements[j]->type) {
                    case ELEMENT_IMAGE: 
                    case ELEMENT_SPRITE: 
                    case ELEMENT_FONT: 
                    {
                        SDL_RenderCopyEx(
                                renderer,
                                texture,
                                NULL,
                                rect,
                                rotation,
                                NULL,
                                SDL_FLIP_NONE
                        );
                        
                        break;
                    }
                    case ELEMENT_RECT: 
                    {
                        ScreenElement *params = update_elements[j];
                        SDL_SetRenderDrawColor(renderer, params->color.r, params->color.g,
                                               params->color.b, params->color.a);
                        int ret = 0;
                        if (params->filled) {
                            ret = SDL_RenderFillRect(renderer, &params->position);
                        } else {
                            ret = SDL_RenderDrawRect(renderer, &params->position);
                        }

                        if (ret < 0) {
                            N2DE_ERROR("failed to draw rect\n");
                            exit(1);
                        }
                        break;
                    }
                    case ELEMENT_LINE: 
                    {
                        ScreenElement *screenElements = update_elements[j];
                        SDL_SetRenderDrawColor(renderer, screenElements->color.r, screenElements->color.g,
                                               screenElements->color.b, screenElements->color.a);

                        int ret = SDL_RenderDrawLine(renderer, screenElements->data.line.start.x,
                                                       screenElements->data.line.start.y, screenElements->data.line.end.x,
                                                       screenElements->data.line.end.y);
                        if (ret < 0) {
                            N2DE_ERROR("failed to draw line\n");
                            exit(1);
                        }
                        break;
                    }
                }
            }
        }
        
    }
    /* printf("update skip_count: %u\n", skip_count); */

}

#endif // N2DE_DECLARE_H_
