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
 * bject
 */
#ifndef N2DE_WORLD_H_
#define N2DE_WORLD_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"

#include "error.h"
#include "files.h"
#include "declare.h"
#include "images.h"
#include "memory.h"
#include "sprites.h"
#include "config.h"
#include "players.h"

#define CROP_VARIATION_COUNT 4

#if defined(_WIN32)
#pragma pack(push, 1)
typedef struct _Object
#elif defined(__GNUC__)
typedef struct __attribute__((__packed__)) _Object
#endif
{
    SDL_Point position;
    SDL_Rect hitbox;
    Size size;
    SDL_Rect crops[CROP_VARIATION_COUNT];
    uint16_t crop_id;
    uint8_t crop_variation: 2;
    char type[25];
    bool is_collidable : 1;
    bool is_visable : 1;
    bool wasCollected : 1;
    uint16_t rotation : 9; // degrees 0 - 511 only
}
Object;
#if defined(_WIN32)
#pragma pack(pop)
#endif



#if  defined(_WIN32)
#pragma pack(push, 1)
typedef struct _Map
#elif defined(__GNUC__)
typedef struct __attribute__((__packed__)) _Map
#endif

{
    uint16_t objects_count;
    char background_path[500];
    uint8_t grid;
    SDL_Point position;
    Size size;
}
Map;
#if defined(_WIN32)
#pragma pack(pop)
#endif


extern bool WORLD_UPDATE;
extern float WORLD_SCALE;

#define MAX_COLLISION_CHECK 20

typedef struct _Collider {
    /* char type[50]; */
    bool is_hit;
    Object *object;
    bool hit_top;
    bool hit_bottom;
    bool hit_left;
    bool hit_right;
} Collider;

typedef struct _FrontObject {
    uint16_t crop_id;
    uint16_t object_id;
    SDL_Rect rect;
} FrontObject;

typedef struct _World
{
    Object *objects;
    /* Object *front_objects; */
    uint16_t front_objects_count;
    FrontObject *front_objects;
    /* uint16_t *front_objects; */
    /* uint16_t *front_objects_id; */
    char path[500];
    Map map;
    uint16_t spritesheet_id;
    uint16_t crop_id_start;
    uint16_t crop_id_end;
    uint8_t userdata[32];
    SDL_Rect collision_checks[MAX_COLLISION_CHECK];
    /* bool collisions[MAX_COLLISION_CHECK]; */
    Collider collisions[MAX_COLLISION_CHECK];
    uint16_t collision_count;
    bool exists;
    bool map_exists;
    bool objects_exists;
    SDL_Texture *backgroundTexture;
    SDL_Rect rect;
} 
World;

void world_save(World *world, const uint8_t map_index)
{

    if (!world->exists) {
        MKDIR(world->path);
    }

    char path[50];
    memset(path, 0, 50);
    char filePath[500];
    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "map");
    getDataPath(path, filePath, map_index);
    FILE *fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "wb+");
#else
    fp = fopen(path, "wb+");
#endif
    if (!fp) {
        N2DE_ERROR("saveMap() error: fopen returned NULL (%d)\n", errno);
    }
    fwrite((void *)&world->map, sizeof(Map), 1, fp);
    fclose(fp);
    memset(path, 0, 50);

    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "objects");
    getDataPath(path,filePath, map_index);

    fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "wb+");
#else
    fp = fopen(path, "wb+");
#endif
    if (!fp) {
        N2DE_ERROR("saveMap() error: fopen returned NULL (%d)\n", errno);
    }
    printf("background image: %s\n", world->map.background_path);
    for (int i = 0; i < world->map.objects_count; ++i) {
        printf("%d | writing type: %s\n", i, world->objects[i].type);
        /* printf("%d | writing position: %d, %d\n", i, world->objects[i].position.x, world->objects[i].position.y); */
        printf("%d | writing size: %d, %d\n", i, world->objects[i].size.w, world->objects[i].size.h);
                printf("%d | writing hitbox: %d, %d, %d, %d\n", 
                        i,
                        world->objects[i].hitbox.x,
                        world->objects[i].hitbox.y,
                        world->objects[i].hitbox.w,
                        world->objects[i].hitbox.h
                );
        uint16_t written = fwrite((void *)&world->objects[i], sizeof(Object),
                1, fp);
        printf("written: %d\n", written);
    }
    fclose(fp);
}

uint16_t world_read(World *world, const uint8_t map_index, SDL_Renderer *renderer, Images *images, Sprites *sprites, int8_t zIndex)
{
    char path[50];
    memset(path, 0, 50);

    char filePath[500];
    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "map");

    getDataPath(path, filePath, map_index);

    world->map_exists = access(path, F_OK) == 0;
    if (!world->map_exists) {
        return 0;
    }

    FILE *fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "rb");
#else
    fp = fopen(path, "rb");
#endif
    if (!fp) {
        N2DE_ERROR("world_read() error: fopen returned NULL (%d)\n",\
                errno);
    }

    uint16_t size = sizeof(Map);
    uint16_t read = fread((void *)&world->map, size, 1, fp);
    char background_path[255];
    sprintf(background_path, RESPATH "/images/%s", world->map.background_path);
    uint16_t images_id = images_newGroup(images);
    /* world->backgorund_id = images_create(images, renderer, background_path, NULL, zIndex); */
    /* images_saveGroup(images, images_id); */
    SDL_Surface *surface = IMG_Load(background_path);
    world->backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (read >= size) {
        N2DE_ERROR("world_read() error: could not read file %s (%d) \n",
                path, errno);
        exit(1);
    }

    fclose(fp);
    memset(path, 0, 50);

    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "objects");

    getDataPath(path, filePath, map_index);
    world->objects_exists = access(path, F_OK) == 0;
    if (!world->objects_exists) {
        return 0;
    }

    fp = NULL;
#if defined (_WIN32)
    fopen_s(&fp, path, "rb");
#else
    fp = fopen(path, "rb");
#endif
    /* fopen_s(&pFile, path, "rb"); */
    if (!fp) {
        N2DE_ERROR("world_read() error: fopen returned NULL (%d)\n",\
                errno);
    }

    size = sizeof(Object) * world->map.objects_count;
    MEMRES(world->objects, size);
    read = 0;

    uint16_t front_objects_count = 0;
    for (uint16_t i = 0; i < world->map.objects_count ; ++i) {
        Object object;
        read = fread((void *)&object, sizeof(Object), 1, fp);
        if (read != 1) {
            N2DE_ERROR("could not read %s\n", path);

        }
        memcpy(&world->objects[i], &object, sizeof(Object));
        world->objects[i].crop_id = sprites_addCrops(sprites, renderer, world->spritesheet_id, world->objects[i].type, world->objects[i].crops,
                CROP_VARIATION_COUNT, zIndex);
        SDL_Rect back = {
            object.crops[0].x,
            object.crops[0].y,
            object.crops[0].w,
            object.crops[0].h,
        };
        SDL_Rect front = {
            object.crops[0].x,
            object.crops[0].y,
            object.crops[0].w,
            object.hitbox.y,
        };
        if (!SDL_RectEquals(&front, &back) && object.is_visable) {
            uint16_t j = front_objects_count++;
            MEMRES(world->front_objects, sizeof(FrontObject) * front_objects_count);
            world->front_objects[j].crop_id = sprites_addCrops(sprites, renderer, world->spritesheet_id, object.type, &front,
                    1, zIndex + 2);
            world->front_objects[j].object_id = i;
            world->front_objects_count = front_objects_count;
            memcpy(&world->front_objects[j].rect, &front, sizeof(SDL_Rect));
        }
    }
    fclose(fp);
    printf("world read\n");
    return size;
    /* return images_id; */
}

uint16_t world_getObjects(World *world, const uint8_t map_index, Object *objects, uint16_t *count)
{
    char path[50];
    memset(path, 0, 50);

    char filePath[500];
    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "map");

    getDataPath(path, filePath, map_index);
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        N2DE_ERROR("world_read() error: fopen returned NULL (%d)\n",\
                errno);
    }

    uint16_t size = sizeof(Map);
    uint16_t read = fread((void *)&world->map, size, 1, fp);

    if (read >= size) {
        N2DE_ERROR("world_read() error: could not read file %s (%d) \n",
                path, errno);
        exit(1);
    }

    fclose(fp);
    memset(path, 0, 50);

    memset(filePath, 0, 500);
    strcat(filePath, world->path);
    strcat(filePath, "objects");

    getDataPath(path, filePath, map_index);
    fp = fopen(path, "rb");

    if (!fp) {
        N2DE_ERROR("world_read() error: fopen returned NULL (%d)\n",\
                errno);
    }

    size = sizeof(Object) * world->map.objects_count;
    MEMRES(objects, size);
    *count =world->map.objects_count;
    read = 0;

    for (uint16_t i = 0; i < world->map.objects_count ; ++i) {
        Object object;
        read = fread((void *)&object, sizeof(Object), 1, fp);
        if (read != 1) {
            N2DE_ERROR("could not read %s\n", path);

        }
        memcpy(&objects[i], &object, sizeof(Object));
    }
    fclose(fp);
    return size;

}

uint16_t world_addCollisionCheck(World *world, SDL_Rect *collision)
{
    uint16_t i = world->collision_count++;
    memcpy(&world->collision_checks[i] , collision, sizeof(SDL_Rect));
    return i;
}

Collider * world_checkCollision(World *world, uint16_t id)
{
    if (world->collisions[id].is_hit) {
        world->collisions[id].is_hit = false;
        return &world->collisions[id];
    }
}

void world_updateQue(World *world, SDL_Renderer *renderer, Images *images,
                     Sprites *sprites, Players *players, SDL_Point map_pos,
                     SDL_Rect *window)
{
    /* return; */
    if (!WORLD_UPDATE) return;
    /* if (world->map.objects_count == 0) return; */
    SDL_Point apos = {
        map_pos.x - players->playerInfo[0].position.x,
        map_pos.y + players->playerInfo[0].position.y,
    };
    SDL_Rect r = {
        .x = apos.x * WORLD_SCALE,
        .y = apos.y * WORLD_SCALE,
        .w = world->map.size.w * WORLD_SCALE,
        .h = world->map.size.h * WORLD_SCALE,
    };
    memcpy(&world->rect, &r, sizeof(SDL_Rect));

    SDL_RenderCopyEx(
            renderer,
            world->backgroundTexture,
            NULL,
            &r,
            0,
            NULL,
            SDL_FLIP_NONE
    );

    /* uint16_t skip_count = 0; */
    for (uint16_t i = 0 ; i < world->map.objects_count; ++i) {
        Object *object = &world->objects[i];
        /* SDL_Rect hitbox = { */
        /*     .x = ((object->position.x + object->hitbox.x) * world->map.grid * WORLD_SCALE) + (apos.x * WORLD_SCALE), */
        /*     .y = ((object->position.y + object->hitbox.y) * world->map.grid * WORLD_SCALE) + (apos.y * WORLD_SCALE), */
        /*     .w = (object->hitbox.w) * WORLD_SCALE, */
        /*     .h = (object->hitbox.h) * WORLD_SCALE, */
        /* }; */
        SDL_Point position = {
            ((object->position.x * world->map.grid) * WORLD_SCALE) + (apos.x * WORLD_SCALE),
            ((object->position.y * world->map.grid) * WORLD_SCALE) + (apos.y * WORLD_SCALE),
        };
        Size size = {
            .w = object->size.w * WORLD_SCALE,
            .h = object->size.h * WORLD_SCALE
        };

        SDL_Rect x = {
            position.x,
            position.y,
            size.w,
            size.h

        };
        SDL_Rect result;
        bool intersect  = SDL_IntersectRect(&x, window, &result);
        if (!intersect) {
            /* skip_count++; */
            continue;
        };
        bool hit_bottom = false;
        for (uint16_t j = 0; j < world->collision_count; ++j) {
            SDL_Rect hitbox = {
                position.x + ((object->hitbox.x * world->map.grid) * WORLD_SCALE),
                position.y + ((object->hitbox.y * world->map.grid) * WORLD_SCALE),
                object->hitbox.w * world->map.grid * WORLD_SCALE,
                object->hitbox.h * world->map.grid * WORLD_SCALE,
            };
            if (SDL_HasIntersection(&world->collision_checks[j], &hitbox)) {
                world->collisions[j].is_hit = true;
                world->collisions[j].object = object;
                SDL_Point top[2] = {
                    {hitbox.x, hitbox.y},
                    {hitbox.x + hitbox.w, hitbox.y}
                };
                world->collisions[j].hit_top = \
                    SDL_IntersectRectAndLine(&world->collision_checks[j], 
                            &top[0].x, &top[0].y, &top[1].x, &top[1].y);

                SDL_Point left[2] = {
                    {hitbox.x, hitbox.y},
                    {hitbox.x, hitbox.y + hitbox.h}
                };

                world->collisions[j].hit_left = \
                    SDL_IntersectRectAndLine(&world->collision_checks[j], 
                            &left[0].x, &left[0].y, &left[1].x, &left[1].y);

                SDL_Point bottom[2] = {
                    {hitbox.x, hitbox.y + hitbox.h},
                    {hitbox.x + hitbox.w, hitbox.y + hitbox.h}
                };

                world->collisions[j].hit_bottom = \
                    SDL_IntersectRectAndLine(&world->collision_checks[j], 
                            &bottom[0].x, &bottom[0].y, &bottom[1].x, &bottom[1].y);

                SDL_Point right[2] = {
                    {hitbox.x + hitbox.w, hitbox.y},
                    {hitbox.x + hitbox.w, hitbox.y + hitbox.h}
                };
                world->collisions[j].hit_right = \
                    SDL_IntersectRectAndLine(&world->collision_checks[j], 
                            &right[0].x, &right[0].y, &right[1].x, &right[1].y);

                SDL_Point bottom2[2] = {
                    {position.x, position.y + size.h},
                    {position.x + size.w, position.y + size.h}
                };
                hit_bottom = \
                    SDL_IntersectRectAndLine(&world->collision_checks[j], 
                            &bottom2[0].x, &bottom2[0].y, &bottom2[1].x, &bottom2[1].y);

            }
        }
        if (object->is_visable) {
            sprites_updateCrop2(sprites, renderer, position, size,
                    0, world->spritesheet_id, object->crop_id);
        }

        for (uint16_t k = 0 ; k < world->front_objects_count; ++k) {
            FrontObject *front_object = &world->front_objects[k];
            if (front_object->object_id == i && !hit_bottom) {
                /* size.w = 0; */
                /* size.h = 0; */
                Size s = {
                    .w = size.w,
                    .h = object->hitbox.y * world->map.grid * WORLD_SCALE
                };
                sprites_updateCrop2(sprites, renderer, position, s,
                        0, world->spritesheet_id, front_object->crop_id);

            }
        }
    }
    /* printf("world skip_count: %u\n", skip_count); */
    WORLD_UPDATE = false;
}

void world_init(World *world, char *path, uint16_t spritesheet_id)
{
    memset(world, 0, sizeof(World));
    memset(world->collisions, 0, sizeof(Collider) * MAX_COLLISION_CHECK);
    world->objects = NULL;
    world->front_objects = NULL;
    world->exists = access(path, F_OK) == 0;
    world->collision_count = 0;
    strcpy(world->path, path);
    world->spritesheet_id = spritesheet_id;
    world->backgroundTexture = NULL;
}

void world_addObject(World *world, Object *object)
{
    uint16_t i = world->map.objects_count++;
    MEMRES(world->objects, sizeof(Object) * world->map.objects_count);
    memcpy(&world->objects[i], object, sizeof(Object));
}

void world_changeObject(World *world, uint16_t id, Object *object)
{
    memcpy(&world->objects[id], object, sizeof(Object));
}

void world_print(World *world)
{
    printf("\nmap grid : %d\n", world->map.grid);
    for (uint16_t i = 0 ; i < world->map.objects_count; ++i) {
        Object *object = &world->objects[i];
        printf("\nobject number: %d\n", i);
        printf("\tposition x: %d\n", world->objects[i].position.x);
        printf("\tposition y: %d\n", world->objects[i].position.y);
        printf("\thitbox x: %d\n", world->objects[i].hitbox.x);
        printf("\thitbox y: %d\n", world->objects[i].hitbox.y);
        printf("\thitbox w: %d\n", world->objects[i].hitbox.w);
        printf("\thitbox h: %d\n", world->objects[i].hitbox.h);
        printf("\tcrops: %d, %d, %d, %d\n",
                object->crops[0].x, object->crops[0].y,
                object->crops[0].w, object->crops[0].h);
        /* printf("\timage file: %s\n", world->objects[i].spritesheet); */
        printf("\tis_collidable: %d\n", world->objects[i].is_collidable);
        printf("\tis_visable: %d\n", world->objects[i].is_visable);
        printf("\twasCollected: %d\n", world->objects[i].wasCollected);
        printf("\trotation: %d\n", world->objects[i].rotation);
        printf("\ttype: %s\n", world->objects[i].type);
        printf("\n");
    }
}

void world_free(World *world)
{
    free(world->objects);
    world->objects = NULL;
    free(world->front_objects);
    world->front_objects = NULL;
    SDL_DestroyTexture(world->backgroundTexture);
    world->backgroundTexture = NULL;
}

#endif // N2DE_WORLD_H_
