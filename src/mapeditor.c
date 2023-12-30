#include <stdio.h>
#include "N2DE/world.h"
#include "N2DE/sprites.h"
#include "N2DE/mapeditor/sdl.h"
#include "N2DE/mapeditor/mapeditor.h"


/* #define OBJECT_COUNT 5 */
int main(int argc, char **argv)
{
    if (argc != 3) {
        N2DE_ERROR("not enough argumets\n");
        return 1;
    }
    MapEditor mapeditor;
    int map_id;
    sscanf(argv[2], "%d", &map_id);
    mapeditor_init(&mapeditor, argv[1], map_id);

    while (!mapeditor.quit) {
        mapeditor_clear(&mapeditor);
        mapeditor_update(&mapeditor);
    }

    mapeditor_quit(&mapeditor);
    return 0;
}
