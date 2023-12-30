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
#ifndef N2DE_FONTS_H_
#define N2DE_FONTS_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"

#include "declare.h"
#include "memory.h"
#include "files.h"

// todo
// create groups for fonts like images.
// groups only store format strings
// the main memory stores the strings
// that get changed every time a state gets changed
typedef struct _Fonts {
    TTF_Font **ttfs;
    uint8_t *update;
    uint8_t update_count;
    ScreenElement *screenElements;
    uint8_t count;
    uint16_t groups_count;
} Fonts;

void fonts_init(Fonts *fonts)
{
    memset(fonts, 0, sizeof(Fonts));
    fonts->screenElements = NULL;
    fonts->update = NULL;
    fonts->update_count = 0;
}

int fonts_create(Fonts *fonts, const char *ttf_path, int size, 
                  SDL_Renderer *renderer, const char *text, int8_t zIndex,
                  SDL_Color fg_color, SDL_Color bg_color)
{

    int i = fonts->count++;

    MEMRES(fonts->screenElements, sizeof(ScreenElement) *
                                   fonts->count);

    MEMRES(fonts->ttfs, sizeof(TTF_Font *) *
                         fonts->count);

    memset(fonts->screenElements[i].data.string, 0, ELEMENT_STRING_SIZE);
    if (strlen(text) == 0) {
        memcpy(fonts->screenElements[i].data.string, "\0", 1);
    } else {

        memcpy(fonts->screenElements[i].data.string, text, strlen(text) * sizeof(char));
    }
    fonts->ttfs[i] = TTF_OpenFont(ttf_path, size);
    fonts->screenElements[i].zIndex = zIndex;

    if (!fonts->ttfs[i]) {
        N2DE_ERROR(TTF_GetError());
        TTF_Quit();
        exit(1);
    }

    fonts->screenElements[i].surface = \
        TTF_RenderText_Shaded(fonts->ttfs[i], text, fg_color, bg_color);

    fonts->screenElements[i].texture = \
        SDL_CreateTextureFromSurface(renderer,
                                     fonts->screenElements[i].surface);

    fonts->screenElements[i].position.w = fonts->screenElements[i].surface->w;
    fonts->screenElements[i].position.h = fonts->screenElements[i].surface->h;
    fonts->screenElements[i].type = ELEMENT_FONT;
    /* memcpy(fonts->screenElements[i].type, "font", 4); */
    /* fonts->screenElements[i].type[4] = '\0'; */
    return i;
}

void fonts_change(Fonts *fonts, int id, SDL_Renderer *renderer, 
                  const char *text, SDL_Color fg_color, SDL_Color bg_color)
{

    if (strlen(text) == 0) {
        return;
    }
    screenElements_free(&fonts->screenElements[id]);


    fonts->screenElements[id].surface= \
        TTF_RenderText_Shaded(fonts->ttfs[id], text, fg_color, bg_color);

    fonts->screenElements[id].texture = \
        SDL_CreateTextureFromSurface(renderer,
                                     fonts->screenElements[id].surface);

    fonts->screenElements[id].position.w = \
        fonts->screenElements[id].surface->w;
    fonts->screenElements[id].position.h = \
        fonts->screenElements[id].surface->h;
}

void fonts_que(Fonts *fonts, uint8_t id, SDL_Point p)
{
    fonts->screenElements[id].position.x = p.x;
    fonts->screenElements[id].position.y = p.y;

    int j = fonts->update_count++;
    MEMRES(fonts->update, sizeof(uint8_t) * fonts->update_count);
    fonts->update[j] = id;
}

void fonts_updateQue(Fonts *fonts, ScreenElement *update_elements[20], uint16_t *count)
{
    if (!fonts->update) return;

    for (uint8_t i = 0; i < fonts->update_count; ++i) {
        uint8_t id = fonts->update[i];
                update_elements[(*count)] = &fonts->screenElements[id];
                update_elements[(*count)]->rotation = 0;
                (*count)++;
    }
}

void fonts_clear(Fonts *fonts)
{
    for (uint8_t i = 0; i < fonts->count; ++i) {
        screenElements_free(&fonts->screenElements[i]);
    }
    free(fonts->screenElements);
    fonts->screenElements = NULL;
    fonts->count = 0;
    /* free(fonts->update); */
    /* fonts->update = NULL; */
    /* fonts->update_count = 0; */
}

void fonts_reset(Fonts *fonts)
{
    free(fonts->update);
    fonts->update = NULL;
    fonts->update_count = 0;

}

SDL_Rect *fonts_getRect(Fonts *fonts, uint8_t id)
{
    if (!fonts->screenElements) return NULL;
    return &fonts->screenElements[id].position;
}

uint16_t fonts_newGroup(Fonts *fonts)
{
    uint16_t i = fonts->groups_count++;
    fonts->count = 0;
    return i;
}

bool fonts_loadGroup(Fonts *fonts, uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/fonts/%u", id);
    if (access(path, F_OK) != 0) {
        return false;
    }
    fonts->count = 0;
    FILE *fp = fopen(path, "rb");
    int read = fread(&fonts->count, sizeof(uint16_t), 1, fp);
    if (read != 1) {
        N2DE_ERROR("could not write to group file\n");
    }
    if (fonts->count == 0) return false;
    MEMRES(fonts->screenElements, sizeof(ScreenElement) * fonts->count);
    read = fread(fonts->screenElements, sizeof(ScreenElement),fonts->count, fp);
    if (read != fonts->count) {
        N2DE_ERROR("could not write to group file\n");
    }
    fclose(fp);
    return true;

}

void fonts_saveGroup(Fonts *fonts, uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/fonts/%u", id);
    FILE *fp = fopen(path, "wb+");
    int write = fwrite(&fonts->count, sizeof(uint16_t), 1, fp);
    if (write != 1) {
        N2DE_ERROR("could not write to group file\n");
    }
    write = fwrite(fonts->screenElements, sizeof(ScreenElement), fonts->count, fp);
    if (write != fonts->count) {
        N2DE_ERROR("could not write to group file\n");
    }
    fclose(fp);
}

void fonts_quit(Fonts *fonts)
{
    for (uint8_t i = 0; i < fonts->count; ++i) {
        /* screenElements_free(&fonts->screenElements[i]); */
        TTF_CloseFont(fonts->ttfs[i]);
    }
    free(fonts->ttfs);
    free(fonts->screenElements);
    free(fonts->update);
}

void fonts_getString(Fonts *fonts, char string[ELEMENT_STRING_SIZE], uint16_t id)
{
    memset(string, 0, ELEMENT_STRING_SIZE);
    memcpy(string, fonts->screenElements[id].data.string, 
            strlen(fonts->screenElements[id].data.string) * sizeof(char));
}

#endif // N2DE_FONTS_H_
