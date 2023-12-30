#ifndef N2DE_MOUSE_H_
#define N2DE_MOUSE_H_

#include "../SDL2/SDL.h"
#include "./declare.h"

typedef struct _Mouse 
{
    SDL_Point relative_position;
    SDL_Point absolute_position;
    SDL_Point grid_position;
    uint32_t button;
    uint8_t quadrant;
    //              |
    //              |
    //  quadrant 2  |  quadrant 1
    //              |
    // ----------------------------
    //              |
    // quadrant 3   | quadrant 4
    //              |
    //              |
    // 0 = middle
    // now I need the x and y from a grid perspective
    int g_x;
    int g_y;
} 
Mouse;

void mouse_get(Mouse *mouse, Size size, int scale, int grid) {
        int x;
        int y;
        mouse->quadrant = 0;
        //
        mouse->button = SDL_GetMouseState(&x, &y);
        mouse->absolute_position.x = x;
        mouse->absolute_position.y = y;
        int rx = ((float)x / (float)(scale));
        int ry = ((float)y / (float)(scale));
        mouse->relative_position.x = rx;
        mouse->relative_position.y = ry;
        int gx = ((float)rx / (float)(grid));
        int gy = ((float)ry / (float)(grid));
        mouse->grid_position.x = gx;
        mouse->grid_position.y = gy;
        int mid_x = size.w / 2;
        int mid_y = size.h / 2;
        if ((x > mid_x) && (y < mid_y)) {
            mouse->quadrant = 1;
        }
        if ((x < mid_x) && (y < mid_y)) {
            mouse->quadrant = 2;
        }
        if ((x < mid_x) && (y > mid_y)) {
            mouse->quadrant = 3;
        }
        if ((x > mid_x) && (y > mid_y)) {

            mouse->quadrant = 4;
        }
        mouse->g_x = x - mid_x;
        mouse->g_y = y - mid_y;
        /* printf("g_x: %d, g_y: %d\n", mouse->g_x, mouse->g_y); */
} 

#endif // N2DE_MOUSE_H_
