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
#ifndef N2DE_WAITING_H_
#define N2DE_WAITING_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "declare.h"
#include "memory.h"
#include "shapes.h"
#include "uuid.h"

typedef struct _Wait {
    uint64_t start;
    uint64_t elapsed;
    uint64_t time;
    char *uuid;
} Wait;

typedef struct _Waiting {
    Wait *waits;
    uint8_t count;
} Waiting;

void waiting_init(Waiting *waiting)
{
    memset(waiting, 0, sizeof(Waiting));
    waiting->count = 0;
    waiting->waits = NULL;
}

char *waiting_new(Waiting *waiting, uint64_t t)
{
    int i = waiting->count++;
    /* MEMRES(waiting->waits, sizeof(Wait) * waiting->count); */
    Wait *wait = &waiting->waits[i];
    /* BIN_UUID_T bin_uuid; */
    /* UUID_CREATE(bin_uuid, wait->uuid); */
    wait->time = t;
    wait->elapsed = 0;
    wait->start = 0;
    return wait->uuid;
}

int waiting_getID(Waiting *waiting, char *uuid) {

    if (!waiting->waits) return -1;
    for (uint8_t i = 0; i < waiting->count; ++i) {
        Wait *wait = &waiting->waits[i];
        if (!strcmp(uuid, wait->uuid)) {
            return i;
        }
    }
    return -1;
}

void waiting_update(Waiting *waiting)
{
    for (int i = 0; i < waiting->count; ++i) {
        Wait *wait = &waiting->waits[i];
        if (wait->elapsed >= wait->time) {

            if (waiting->count - 1 == 0) {
                UUID_FREE(waiting->waits[0].uuid);
                free(waiting->waits);
                waiting->waits = NULL;
                waiting->count = 0;
                return;
            }

            Wait *newWaits = NULL;

            MEMRES(newWaits, sizeof(Wait) * (waiting->count - 1));

            int j = 0;

            for (uint8_t k = 0; k < waiting->count; ++k) {
                Wait *oldWait = &waiting->waits[k];
                if (!strcmp(wait->uuid, oldWait->uuid)) {
                    UUID_FREE(oldWait->uuid);
                    continue;
                }
                memcpy(&newWaits[j], oldWait, sizeof(Wait));
                j++;
            }

            free(waiting->waits);
            waiting->waits = newWaits;
            waiting->count--;
            return;
        }

        if (wait->start == 0) {
            wait->start = SDL_GetTicks64();
        } else {
            wait->elapsed = SDL_GetTicks64() - wait->start;
        }
    }
}

bool waiting_finished(Waiting *waiting, char *uuid)
{
    if (!uuid || !waiting->waits) return true;

    for (int i = 0; i < waiting->count; ++i) {
        Wait *wait = &waiting->waits[i];
        if (!strcmp(wait->uuid, uuid)) {
            return false;
        }
    } 

    return true;
}

void waiting_free(Waiting *waiting)
{
    for (uint8_t i = 0; i < waiting->count; ++i) {
        Wait *wait = &waiting->waits[i];
        UUID_FREE(wait->uuid);
    }

    free(waiting->waits);
}

#endif // N2DE_WAITING_H_
