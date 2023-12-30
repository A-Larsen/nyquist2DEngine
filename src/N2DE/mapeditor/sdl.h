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
#ifndef N2DE_MAPEDITOR_SDL_H_
#define N2DE_MAPEDITOR_SDL_H_

#include <string.h>

#include "../declare.h"
#include "../error.h"

typedef struct _SDLData {
    SDL_Rect rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Surface *window_surface;
    Size size;
    uint8_t scale;
} SDLData;

void sdl_init(SDLData *data, Size *size, uint8_t scale)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    N2DE_ERROR("could not initialize SDL");
        exit(1);
    }

    memcpy(&data->size, size, sizeof(Size));

    data->scale = scale;
    data->window = SDL_CreateWindow("mapeditor", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, 
                                      size->w * scale, size->h * scale, 0);
    if (!data->window) {
        N2DE_ERROR("could not creat a window\n%s", SDL_GetError());
    }

    data->window_surface = SDL_GetWindowSurface(data->window);


    data->renderer = SDL_CreateRenderer(data->window, -1,\
                                           SDL_RENDERER_ACCELERATED);

    if (!data->renderer) {
        N2DE_ERROR("could not create renderer\n%s", SDL_GetError());
    }

    int ret = SDL_RenderSetLogicalSize(data->renderer, size->w, size->h);

    if (ret < 0) {
        N2DE_ERROR("could not set logical size\n%s", SDL_GetError());
    }

    SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
    SDL_RenderClear(data->renderer);
    SDL_RenderPresent(data->renderer);
}

void sdl_quit(SDLData *data) {
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
    SDL_Quit();
}

#endif // N2DE_MAPEDITOR_SDL_H_
