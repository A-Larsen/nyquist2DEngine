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
#ifndef N2DE_AUDIO_H_
#define N2DE_AUDIO_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"
#include "../SDL2/SDL_mixer.h"

#include "declare.h"
#include "memory.h"
#include "error.h"
#include "memory.h"


typedef struct _WavFile {
    char name[20];
    char path[50];
    int loop;
} WavFile;

typedef struct _WavInfo {
    uint8_t *wavStart;
    uint8_t *wavPos;
    uint32_t wavLength;
    bool stop;
    SDL_AudioSpec wavSpec;
    SDL_AudioDeviceID device_id;
    WavFile wav;
} WavInfo;

typedef struct _Audio {
    WavInfo *wavInfo;
    int wavs_count;
} Audio;

// typedef struct _AudioData {
//     uint8_t *pos;
//     uint32_t length;
// } AudioData;

static void callback(void * userdata, uint8_t * stream, int streamLength)
{
    WavInfo *wavInfo = (WavInfo *)userdata;
    SDL_LockAudioDevice(wavInfo->device_id);
    memset(stream, 0, streamLength);
    if (wavInfo->stop) {
        wavInfo->wavPos = wavInfo->wavStart;
        return;
    }
    uint32_t pos = (wavInfo->wavPos - wavInfo->wavStart);
    uint32_t remaining = wavInfo->wavLength - pos;
    uint32_t length = (uint32_t)streamLength;
    if (wavInfo->wav.loop) {
        uint32_t newpos = pos + length;
        if (newpos > wavInfo->wavLength) {
            wavInfo->wavPos = wavInfo->wavStart;
            return;
        } else {
           memcpy(stream, wavInfo->wavPos, length);
           wavInfo->wavPos += length;
        }
    } else {
        if ((long int)remaining < -pos) {
            return;
        }
        length = (length > remaining ? remaining : length);
        memcpy(stream, wavInfo->wavPos, length);
        wavInfo->wavPos += length;
    }
    SDL_UnlockAudioDevice(wavInfo->device_id);
}

void audio_init(Audio *audio, WavFile *wav_files, int size)
{
    if (audio->wavInfo) {
        for (uint8_t i = 0; i < audio->wavs_count; ++i) {
            audio->wavInfo[i].wavPos = audio->wavInfo[i].wavStart;
            SDL_PauseAudioDevice(audio->wavInfo[i].device_id, 1);
            SDL_FreeWAV(audio->wavInfo[i].wavStart);
            SDL_CloseAudioDevice(audio->wavInfo[i].device_id);
        }
    }
    audio->wavInfo = NULL;
    MEMRES(audio->wavInfo, sizeof(WavInfo) * size);
    for (uint8_t i = 0; i < size; ++i) {
        memcpy(&audio->wavInfo[i].wav, &wav_files[i], sizeof(WavFile));
        audio->wavInfo[i].stop = false;
        if( 
            SDL_LoadWAV(
                audio->wavInfo[i].wav.path, &audio->wavInfo[i].wavSpec, 
               &audio->wavInfo[i].wavStart,
               &audio->wavInfo[i].wavLength
            ) == NULL
        ) {
            N2DE_ERROR("could not load audo file %s\n", audio->wavInfo[i].wav.path);
        }
        audio->wavInfo[i].stop = false;
        audio->wavInfo[i].wavPos = audio->wavInfo[i].wavStart;
        audio->wavInfo[i].wavSpec.callback = callback;
        audio->wavInfo[i].wavSpec.userdata = &audio->wavInfo[i];
        /* audio->wavInfo[i].device_id = SDL_OpenAudioDevice(NULL, 0, &audio->wavInfo[i].wavSpec, */ 
        /*                                        NULL, SDL_AUDIO_ALLOW_ANY_CHANGE); */
        audio->wavInfo[i].device_id = SDL_OpenAudioDevice(NULL, 0, &audio->wavInfo[i].wavSpec, 
                                               NULL, 0);
        if (audio->wavInfo[i].device_id == 0) {
            N2DE_ERROR("could not load an audio device\n%s", SDL_GetError());
            exit(1);
        }
    }
    // audio->device_id = SDL_OpenAudioDevice(NULL, 0, &audio->wavSpec, 
    //                                        NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    audio->wavs_count = size;
}

int audio_getIdByName(Audio *audio, char *name)
{
    for(uint8_t i = 0; i < audio->wavs_count; ++i) {
        if (strcmp(name, audio->wavInfo[i].wav.name) == 0) {
            return i;
        }
    }
    return -1;
}

void audio_play(Audio *audio, int id)
{
    // audiodata.pos = audio->wavInfo[id].wavStart;
    // audiodata.length = audio->wavInfo[id].wavLength;
    SDL_PauseAudioDevice(audio->wavInfo[id].device_id, 0);
    // need to make an sdl thread to play audio
}

void audio_stopAll(Audio *audio)
{
    for(uint8_t i = 0; i < audio->wavs_count; ++i) {
        // audio->wavInfo[i].stop = true;
       //  SDL_CloseAudioDevice(audio->wavInfo[i].device_id);
        audio->wavInfo[i].wavPos = audio->wavInfo[i].wavStart;
        SDL_PauseAudioDevice(audio->wavInfo[i].device_id, 1);
    }
}

void audio_quit(Audio *audio)
{
    for(uint8_t i = 0; i < audio->wavs_count; ++i) {
        SDL_FreeWAV(audio->wavInfo[i].wavStart);
        SDL_CloseAudioDevice(audio->wavInfo[i].device_id);
    }
}


#endif // N2DE_AUDIO_H_
