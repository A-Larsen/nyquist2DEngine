#include "./src/lua5.3/lua.h"
#include "./src/lua5.3/lauxlib.h"
#include "./src/N2DE/N2DE.h"

int libworld_init(lua_State *L)
{

    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *spritesheet = (char *)luaL_checkstring(L, 1);
    char *world = (char *)luaL_checkstring(L, 2);
    char spritesheet_path[255];
    sprintf(spritesheet_path, RESPATH "/images/%s", spritesheet);
    Size size = {.w = 1, .h = 1};
    /* uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path, */
    /*                           NULL, 0); */
    uint8_t id = sprites_new(&engine->sprites, engine->renderer, spritesheet_path);
    char world_path[255];
    sprintf(world_path, RESPATH "/data/%s/", world);

    world_init(&engine->world, world_path, id);
    return 0;
}

int libworld_read(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    int id = (int)luaL_checknumber(L, 1);
    int zIndex = (int)luaL_checknumber(L, 2);
    uint16_t group_id = world_read(&engine->world, id, engine->renderer, &engine->images, &engine->sprites, zIndex);
    return group_id;
}

int libworld_print(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    world_print(&engine->world);
    return 0;
}

int libworld_update(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    float scale = (float)luaL_checknumber(L, 1);
    WORLD_UPDATE = true;
    WORLD_SCALE = scale;

    /* world_update(&engine->world, engine->renderer, &engine->sprites); */
    return 0;
}

int libworld_addCollisionCheck(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    SDL_Point p;
    Size s;
    lua_topoint(L, 1, &p);
    lua_tosize(L, 2, &s);
    SDL_Rect r = {
        p.x, p.y, s.w, s.h
    };
    uint16_t i = world_addCollisionCheck(&engine->world, &r);
    lua_pushnumber(L, i);
    return 1;
}

int libworld_checkCollision(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint16_t id = (uint16_t)luaL_checknumber(L, 1);
    Collider *collider = NULL;
    collider = world_checkCollision(&engine->world, id);

    lua_newtable(L);
    if (!collider) {
        lua_pushstring(L, "type");
        lua_pushstring(L, "NONE");
        lua_settable(L, -3);

        lua_pushstring(L, "hit_top");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_right");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_bottom");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_left");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "is_collidable");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, 0);
        lua_settable(L, -3);
        return 1;
    }

        lua_pushstring(L, "type");
        lua_pushstring(L, collider->object->type);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_top");
        lua_pushboolean(L, collider->hit_top);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_right");
        lua_pushboolean(L, collider->hit_right);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_bottom");
        lua_pushboolean(L, collider->hit_bottom);
        lua_settable(L, -3);

        lua_pushstring(L, "hit_left");
        lua_pushboolean(L, collider->hit_left);
        lua_settable(L, -3);

        lua_pushstring(L, "is_collidable");
        lua_pushboolean(L, collider->object->is_collidable);
        lua_settable(L, -3);

        lua_pushstring(L, "is_visable");
        lua_pushboolean(L, collider->object->is_visable);
        lua_settable(L, -3);

    return 1;
}

int libworld_getGrid(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    uint8_t grid = engine->world.map.grid;
    lua_pushnumber(L, grid);
    return 1;
}

int libworld_free(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    world_free(&engine->world);
    return 0;
}

luaL_Reg libworld[] = {
    {"init", libworld_init},
    {"read", libworld_read},
    {"print", libworld_print},
    {"update", libworld_update},
    {"addCollisionCheck", libworld_addCollisionCheck},
    {"checkCollision", libworld_checkCollision},
    {"getGrid", libworld_getGrid},
    {"free", libworld_free},
    {NULL, NULL}
};

int luaopen_extensions_libworld(lua_State *L)
{
    luaL_newlib(L, libworld);
    return 1;
}
