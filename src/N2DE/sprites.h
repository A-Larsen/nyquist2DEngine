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
#ifndef N2DE_SPRITES_H_
#define N2DE_SPRITES_H_

#include "declare.h"
#include "images.h" // Extension of images.h
/* #include "uuid.h" // Extension of images.h */
#include <stdint.h>

// This will be like image.h be will include things like player animation and
// hitboxes

typedef struct _Crops {
    char type[25];
    SDL_Rect *frames;
    ScreenElement *screenElements;
    int count;
    int current_frame;
} Crops;

typedef struct _Sprites {
    Images images;
    Crops ***crops;
    int *crops_count;
    uint16_t count;
    uint16_t *update;
    uint16_t update_count;
} Sprites;

void sprites_init(Sprites *sprites)
{
    memset(sprites, 0, sizeof(Sprites));
    images_init(&sprites->images);
    sprites->crops = NULL;
    sprites->crops_count = NULL;
    sprites->count = 0;
    sprites->update = NULL;
}

void sprites_que(Sprites *sprites, uint16_t id, SDL_Point *p)
{
    Images *images = &sprites->images;
    ScreenElement *screenElement = &images->screenElements[id];
    screenElement->position.x = p->x;
    screenElement->position.y = p->y;

    uint16_t j = sprites->update_count++;
    MEMRES(sprites->update, sizeof(uint16_t) * sprites->update_count);
    sprites->update[j] = id;
}

uint16_t sprites_new(Sprites *sprites, SDL_Renderer *renderer,
                     const char *path)
{
    uint16_t i = sprites->count++;
    /* images_create(&sprites->images, renderer, path, rect, zIndex); */
    images_create(&sprites->images, renderer, path, NULL, 0);


    MEMRES(sprites->crops, sizeof(Crops **) * sprites->count);
    sprites->crops[i] = NULL;
    MEMRES(sprites->crops_count, sizeof(int) * sprites->count);
    sprites->crops_count[i] = 0;


    return i;
}

int sprites_addCrops(Sprites *sprites, SDL_Renderer *renderer, uint16_t id, char *type,
                          SDL_Rect *frames, uint16_t size, uint8_t zIndex)
{
    int i = sprites->crops_count[id]++;

    MEMRES(sprites->crops[id], sizeof(Crops *) * 
                                sprites->crops_count[id]);

    sprites->crops[id][i] = NULL;
    MEMRES(sprites->crops[id][i], sizeof(Crops) * size);
    Crops *crop = sprites->crops[id][i];
    memset(crop, 0, sizeof(Crops));
    memcpy(crop->type, type, strlen(type));
    crop->frames = NULL;
    MEMRES(crop->frames, size * sizeof(SDL_Rect));
    memcpy(crop->frames, frames, size * sizeof(SDL_Rect));
    crop->screenElements = NULL;
    MEMRES(crop->screenElements, size * sizeof(ScreenElement));
    for (uint16_t i =0; i < size; ++i) {
        /* crop->screenElements[i].texture = NULL; */
        SDL_Rect *frame = &crop->frames[i];
        ScreenElement *screenElement = &crop->screenElements[i];

        int w = frame->w;
        int h = frame->h;

        SDL_Rect rect1 = {
            frame->x,
            frame->y,
            frame->w,
            frame->h,
        };

        SDL_Rect rect2 = { 
            0, 
            0, 
            frame->w,
            frame->h,
        };

        SDL_Surface *old_surf = sprites->images.screenElements[id].surface;
        SDL_Surface * surface = SDL_ConvertSurface(old_surf, old_surf->format, old_surf->flags);
        SDL_Rect r = {
            .x = 0,
            .y = 0,
            .w = surface->w,
            .h = surface->h
        };
        SDL_FillRect(surface, &r, 0x00000000);
        SDL_BlitSurface(sprites->images.screenElements[id].surface, &rect1,
                    surface, &rect2);
        surface->w = w;
        surface->h = h;
        surface->w = w;
        surface->h = h;

        screenElement->texture = \
            SDL_CreateTextureFromSurface(renderer, surface);
        screenElement->surface = surface;
        crop->screenElements[i].zIndex = zIndex;
        crop->screenElements[i].type = ELEMENT_SPRITE;
        crop->current_frame = -1;
    }
    crop->count = size;
    return i;
}


Crops *sprites_findCrops(Sprites *sprites, uint16_t id, char *type)
{
    int crops_count = sprites->crops_count[id];
    for (int i = 0; i < crops_count; ++i) {
        Crops *crop = sprites->crops[id][i];
        if(!strcmp(crop->type, type)) {
            return crop;
        }
    }
    return NULL;
}
Crops *sprites_getCrops(Sprites *sprites, uint16_t id, int id2)
{
    return sprites->crops[id][id2];
}

void sprites_updateCrop(Sprites *sprites, SDL_Renderer *renderer,
                    SDL_Point position, Size size, Size scale, int frame_number, uint16_t id,
                    char *type)
{
    Crops *crop = sprites_findCrops(sprites, id, type);
    if (frame_number >= crop->count) return;
    crop->current_frame = frame_number;
    SDL_Rect *frame = &crop->frames[frame_number];
    ScreenElement *screenElement = &crop->screenElements[frame_number];

    /* int w = frame->w; */
    /* int h = frame->h; */

/* /1* } *1/ */
    /* SDL_Rect rect1 = { */
    /*     frame->x, */
    /*     frame->y, */
    /*     frame->w, */
    /*     frame->h, */
    /* }; */

    /* SDL_Rect rect2 = { */ 
    /*     0, */ 
    /*     0, */ 
    /*     frame->w, */
    /*     frame->h, */
    /* }; */


    screenElement->rotation = 0;
    screenElement->position.x = position.x;
    screenElement->position.y = position.y;
    screenElement->position.w = size.w ? size.w : frame->w * scale.w;
    screenElement->position.h = size.h ? size.h : frame->h * scale.h;

    sprites_que(sprites, id, &position);
}

void sprites_updateCrop2(Sprites *sprites, SDL_Renderer *renderer,
        SDL_Point position, Size scale, int frame_number, uint16_t id,
        int id2)
{
    Crops *crop = sprites->crops[id][id2];
    /* if (frame_number >= crop->count) return; */
    crop->current_frame = frame_number;
    SDL_Rect *frame = &crop->frames[frame_number];
    ScreenElement *screenElement = &crop->screenElements[frame_number];

    /* int w = frame->w; */
    /* int h = frame->h; */

    /* SDL_Rect rect1 = { */
    /*     frame->x, */
    /*     frame->y, */
    /*     frame->w, */
    /*     frame->h, */
    /* }; */

    /* SDL_Rect rect2 = { */ 
    /*     0, */ 
    /*     0, */ 
    /*     frame->w, */
    /*     frame->h, */
    /* }; */

    screenElement->rotation = 0;
    screenElement->position.x = position.x;
    screenElement->position.y = position.y;
    screenElement->position.w = scale.w ? scale.w : frame->w;
    screenElement->position.h = scale.h ? scale.h : frame->h;

    sprites_que(sprites, id, &position);
}


void sprites_updateCrop3(Sprites *sprites, SDL_Renderer *renderer,
        SDL_Point position, Size scale, int frame_number, uint16_t id,
        int id2, uint16_t zIndex)
{
    /* Crops *crop = sprites_findCrops(sprites, id, type); */
    /* Crops *crop sprite- */
    Crops *crop = sprites->crops[id][id2];
    if (frame_number >= crop->count) return;
    crop->current_frame = frame_number;
    SDL_Rect *frame = &crop->frames[frame_number];
    ScreenElement *screenElement = &crop->screenElements[frame_number];
    screenElement->zIndex = zIndex;

    /* int w = frame->w; */
    /* int h = frame->h; */

    /* SDL_Rect rect1 = { */
    /*     frame->x, */
    /*     frame->y, */
    /*     frame->w, */
    /*     frame->h, */
    /* }; */

    /* SDL_Rect rect2 = { */ 
    /*     0, */ 
    /*     0, */ 
    /*     frame->w, */
    /*     frame->h, */
    /* }; */

    screenElement->rotation = 0;
    screenElement->position.x = position.x;
    screenElement->position.y = position.y;
    screenElement->position.w = scale.w ? scale.w : frame->w;
    screenElement->position.h = scale.h ? scale.h : frame->h;

    sprites_que(sprites, id, &position);
}

int sprites_animate(Sprites *sprites, SDL_Renderer *renderer,
        SDL_Point position, Size scale, double deltaTime, double speed, uint16_t id,
        char *type)
{
    static double frame_number = 0;

    Crops *crop = sprites_findCrops(sprites, id, type);

    // make sure to not access a frame that doesn't exist
    if (frame_number >= crop->count) {
        frame_number = 0;
    }

    crop->current_frame = frame_number;
    if (crop->count == 1) {
        Size s = {1, 1};
        sprites_updateCrop(sprites, renderer, position, scale, s, 0, id, type);
    } else if ((int)frame_number < crop->count) {
        SDL_Rect *frame = &crop->frames[(int)frame_number];
        ScreenElement *screenElement = &crop->screenElements[(int)frame_number];
        /* int w = frame->w; */
        /* int h = frame->h; */

        /* SDL_Rect rect1 = { */
        /*     frame->x, */
        /*     frame->y, */
        /*     frame->w, */
        /*     frame->h, */
        /* }; */
        /* SDL_Rect rect2 = { */ 
        /*     0, */ 
        /*     0, */ 
        /*     frame->w, */
        /*     frame->h, */
        /* }; */


        screenElement->position.x = position.x;
        screenElement->position.y = position.y;

        screenElement->position.w = scale.w ? scale.w : frame->w;
        screenElement->position.h = scale.h ? scale.h : frame->h;

        sprites_que(sprites, id, &position);
    }

    double inc = deltaTime  * (speed / crop->count);
    if (frame_number + inc < crop->count) {
        frame_number += inc;
    } else {
        frame_number = 0;
    }

    return frame_number;
}

void sprites_resetCrop(Sprites *sprites, uint16_t id, int frame_number)
{
    sprites->crops[id][frame_number]->current_frame = -1;
}

void sprites_rotateCrop(Sprites *sprites, uint16_t id, uint32_t rotation)
{
    sprites->images.screenElements[id].rotation = rotation;
}

void sprites_updateQue(Sprites *sprites, ScreenElement *update_elements[20], uint16_t *count)
{

    if (!sprites->update) return;

    Images *images = &sprites->images;

    for (uint16_t i = 0; i < sprites->update_count; ++i) {
        uint16_t id = sprites->update[i];
        uint32_t rotationInDegrees = images->screenElements[id].rotation;

        for (int j = 0; j < sprites->crops_count[id]; ++j) {
            Crops *crop = sprites->crops[id][j];
            int current_frame = crop->current_frame;
            if (current_frame < 0) continue;
            ScreenElement *screenElement = \
                                           &crop->screenElements[current_frame];

            update_elements[(*count)] = screenElement;
            update_elements[(*count)]->rotation = rotationInDegrees;
            (*count)++;
            sprites_resetCrop(sprites, id, j);
        }
    }
}

void sprites_reset(Sprites *sprites)
{
    free(sprites->update);
    sprites->update = NULL;
    sprites->update_count = 0;
}

void sprites_getRect(SDL_Rect *rect, Sprites *sprites, uint16_t id, char *type, uint8_t frame)
{
    Crops *crop = sprites_findCrops(sprites, id, type);
    SDL_Rect *r = &crop->frames[frame];
    rect->x = 0;
    rect->h = 0;
    rect->w = 0;
    rect->h = 0;
    if (crop) {
        if(crop->screenElements[frame].surface) {
            rect->x = r->x;
            rect->h = r->y;
            rect->w = r->w;
            rect->h = r->h;

        }
    }
}

void sprites_free(Sprites *sprites)
{
    uint16_t size = sprites->count;
    for (uint16_t i = 0; i < size; ++i) {
        if (sprites->crops[i]) {
            for (int j = 0; j < sprites->crops_count[i]; ++j) {
                Crops *crop = sprites->crops[i][j];
                for (int k = 0; k < crop->count; ++k) {
                    screenElements_free(&crop->screenElements[k]);
                }
                free(crop->screenElements);
                free(crop->frames);
                free(crop);
            }
        }
        free(sprites->crops[i]);
    }

    free(sprites->crops);
    sprites->crops = NULL;
    free(sprites->crops_count);
    sprites->crops_count = NULL;
    images_quit(&sprites->images);
    free(sprites->update);
    sprites->update = NULL;
    sprites->count = 0;
    sprites->update_count = 0;
}

#endif // N2DE_SPRITES_H_
