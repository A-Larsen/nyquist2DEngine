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
#ifndef N2DE_LOOPS_H_
#define N2DE_LOOPS_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"

#include "declare.h"
#include "memory.h"

typedef struct _LoopInfo {
    // loop per second
    uint64_t lps;
    // milliseconds per loop
    uint64_t mspl;
    uint64_t frequency;
    uint64_t frequencyAim;
    uint8_t platformOffset;
    uint64_t elapsedTime;
    uint64_t startTime;
    
} LoopInfo;

typedef struct _Loops {
    LoopInfo *loopInfo;
    uint8_t loop_count;
} Loops;

void loops_init(Loops *loops)
{
    memset(loops, 0, sizeof(Loops));
    loops->loop_count = 0;
    loops->loopInfo = NULL;
} 

int loops_add(Loops *loops, uint64_t lps)
{
    loops->loop_count++;
    int i = loops->loop_count - 1;
    MEMRES(loops->loopInfo, sizeof(LoopInfo) * loops->loop_count);
    loops->loopInfo[i].lps = lps;
    loops->loopInfo[i].mspl = (1.0L / loops->loopInfo[i].lps) * 1000;
    loops->loopInfo[i].frequency = SDL_GetPerformanceFrequency();
    loops->loopInfo[i].frequencyAim = 10000000UL;
    loops->loopInfo[i].platformOffset = \
        loops->loopInfo[i].frequency > loops->loopInfo[i].frequencyAim ? 1 : 0;
    loops->loopInfo[i].elapsedTime = 0;
    return i;
}

void loops_start(Loops *loops, int id)
{
    loops->loopInfo[id].startTime = SDL_GetTicks64();
}

void loops_end(Loops *loops, int id)
{
    uint64_t currentTime = SDL_GetTicks64();

    loops->loopInfo[id].elapsedTime = \
        currentTime - loops->loopInfo[id].startTime;

    if  (loops->loopInfo[id].mspl > loops->loopInfo[id].elapsedTime) {
        SDL_Delay((loops->loopInfo[id].mspl - 
                   loops->loopInfo[id].elapsedTime) + 
                   loops->loopInfo[id].platformOffset);
    } else {
        SDL_Delay((loops->loopInfo[id].mspl) + 
                  loops->loopInfo[id].platformOffset);
    }
}

void loops_free(Loops *loops)
{
    free(loops->loopInfo);
}

#endif // N2DE_LOOPS_H_
