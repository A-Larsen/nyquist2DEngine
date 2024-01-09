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
#ifndef N2DE_IMAGES_H_
#define N2DE_IMAGES_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_image.h"

#include "declare.h"
#include "memory.h"
#include "config.h"
#include "files.h"

typedef struct _Images {
    uint8_t *update;
    uint8_t update_count;
    ScreenElement *screenElements;
    uint16_t count;
    uint16_t groups_count;
    // indexes for the stat of each group
    /* uint16_t groups_count; */
} Images;


void images_init(Images *images) {
    memset(images, 0, sizeof(Images));
    images->screenElements = NULL;
    images->update = NULL;
}
void images_quit(Images *images)
{
    /* for (uint8_t i = 0; i < images->count; ++i) { */
    /*     screenElements_free(&images->screenElements[i]); */
    /* } */
    free(images->screenElements);
    images->screenElements = NULL;
    free(images->update);
    images->update = NULL;
    images->count = 0;
    images->update_count = 0;
}

int images_create(Images *images, SDL_Renderer *renderer,
                   const char *images_path, SDL_Rect *rect, uint16_t zIndex)
{
    SDL_Surface *surface = IMG_Load(images_path);

    if (!surface) {
        N2DE_ERROR("Unable to load images from '%s'!\n%s", images_path,
                   IMG_GetError());
        IMG_Quit();
        exit(1);
    } 

    int i = images->count++;
    MEMRES(images->screenElements, sizeof(ScreenElement) *
           images->count);


    ScreenElement *screenElement = &images->screenElements[i];
    screenElement->texture = \
        SDL_CreateTextureFromSurface(renderer, surface);

    if (rect) {
        screenElement->data.rect.x = rect->x;
        screenElement->data.rect.y = rect->y;
        screenElement->data.rect.w = rect->w ? rect->w : surface->w;
        screenElement->data.rect.h = rect->h ? rect->h : surface->h;
    } else {
        screenElement->data.rect.x = 0;
        screenElement->data.rect.y = 0;
        screenElement->data.rect.w = surface->w;
        screenElement->data.rect.h = surface->h;
    }

    screenElement->surface = surface;
    /* SDL_Rect *r = &screenElement->data.rect; */

    screenElement->rotation = 0;
    screenElement->zIndex = zIndex;
    screenElement->type = ELEMENT_IMAGE;
    return i;
}

uint16_t images_newGroup(Images *images)
{
    uint16_t i = images->groups_count++;
    images->count = 0;
    return i;
}

bool images_loadGroup(Images *images, uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/images/%u", id);
    if (access(path, F_OK) != 0) {
        return false;
    }
    images->count = 0;

    FILE *fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "rb");
#else
    fp = fopen(path, "rb");
#endif

    int read = fread(&images->count, sizeof(uint16_t), 1, fp);
    if (read != 1) {
        N2DE_ERROR("could not write to group file\n");
    }
    if (images->count == 0) return false;
    MEMRES(images->screenElements, sizeof(ScreenElement) * images->count);
    read = fread(images->screenElements, sizeof(ScreenElement),images->count, fp);
    if (read != images->count) {
        N2DE_ERROR("could not write to group file\n");
    }
    fclose(fp);
    printf("loaded images group: %d\n", id);
    printf("images screenElements count: %d\n", images->count);
    return true;

}

void images_saveGroup(Images *images, uint16_t id)
{
    char path[255];
    memset(path, 0, 255);
    sprintf(path, "./.cache/images/%u", id);

    FILE *fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "wb+");
#else
    fp = fopen(path, "wb+");
#endif

    int write = fwrite(&images->count, sizeof(uint16_t), 1, fp);
    if (write != 1) {
        N2DE_ERROR("could not write to group file\n");
    }
    write = fwrite(images->screenElements, sizeof(ScreenElement), images->count, fp);
    if (write != images->count) {
        N2DE_ERROR("could not write to group file\n");
    }
    fclose(fp);
    printf("saved images group: %d\n", id);
}

void images_rotate(Images *images, int id, uint32_t rotation)
{
    images->screenElements[id].rotation = rotation;
    /* char path[255]; */
}

void images_que(Images *images, uint8_t id, SDL_Rect *r)
{
    int j = images->update_count++;
    SDL_Rect *position = &images->screenElements[id].position;

    memcpy(position, &images->screenElements[id].data.rect, sizeof(SDL_Rect));

    if (r) {
        position->x = r->x;
        position->y = r->y;
        if (r->h)
            position->w = r->w;
        if (r->h)
            position->h = r->h;
    }

    MEMRES(images->update, sizeof(uint8_t) * images->update_count);
    images->update[j] = id;
}

void images_updateQue(Images *images, ScreenElement *update_elements[20], uint16_t *count)
{
    if (!images->update) return;

    for (uint8_t i = 0; i < images->update_count; ++i) {
        uint8_t id = images->update[i];
        ScreenElement *screenElement = &images->screenElements[id];
        /* uint32_t rotationInDegrees = screenElement->rotation; */
        update_elements[(*count)] = screenElement;
        /* update_elements[(*count)]->rotation = rotationInDegrees; */
        (*count)++;
    }
}

void images_reset(Images *images)
{
    free(images->update);
    images->update = NULL;
    images->update_count = 0;
}

#endif // N2DE_IMAGES_H_
