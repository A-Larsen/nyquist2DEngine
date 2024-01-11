#include "./src/lua5.4/lua.h"
#include "./src/lua5.4/lauxlib.h"
#include "./src/N2DE/N2DE.h"

typedef struct _insert_data {
    int index;
    lua_State *state;
    char *sql;
} _insert_data;

typedef struct _update_data {
    int index;
    lua_State *state;
    char *sql;
} _update_data;

__declspec(dllexport) int libdatabase_init(lua_State *L) {
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char path[255];
    const char *str = luaL_checkstring(L, 1);
    sprintf(path, RESPATH "/%s", str);
    database_init(&engine->database, path);
    Schema *schema = NULL;

    if (!lua_istable(L, 2)) {
        N2DE_ERROR("is not a table");
        exit(1);
    }

    lua_pushnil(L);
    int i = 0;
    while(lua_next(L, 2)) {
        int j = 0;
        char *name  = (char *)luaL_checkstring(L, 3);
        MEMRES(schema, sizeof(Schema) * (i + 1));
        schema[i].kv = NULL;
        memset(schema[i].name, 0, 50);
        memcpy(schema[i].name, name, strlen(name));

        if(!lua_istable(L, 4)) {
            N2DE_ERROR("table malformed");
            exit(1);
        }

        lua_pushnil(L);
        while(lua_next(L, 4)) {
            int value_type = lua_type(L, -1);
            switch (value_type) {
            case LUA_TSTRING: {
                char *key  = (char *)luaL_checkstring(L, -2);
                char *foreign_key  = (char *)luaL_checkstring(L, -1);

                MEMRES(schema[i].kv, sizeof(SchemaKV) * (j + 1));
                memset(&schema[i].kv[j], 0, sizeof(SchemaKV));

                int len = strlen(key);
                memset(schema[i].kv[j].name, 0, len + 1);
                memcpy(schema[i].kv[j].name, key, len);

                len = strlen(foreign_key);
                memset(schema[i].kv[j].foreign, 0, len + 1);
                memcpy(schema[i].kv[j].foreign, foreign_key, len);

                schema[i].kv[j].type = SQLITE_FOREIGN;
                break;
            }
            case LUA_TNUMBER: {
                char *key  = (char *)luaL_checkstring(L, -2);
                int value  = (int)luaL_checknumber(L, -1);
                MEMRES(schema[i].kv, sizeof(SchemaKV) * (j + 1));
                memset(&schema[i].kv[j], 0, sizeof(SchemaKV));
                int len = strlen(key);
                memset(schema[i].kv[j].name, 0, len + 1);
                memcpy(schema[i].kv[j].name, key, len);

                memset(schema[i].kv[j].foreign, 0, sizeof(char) * 50);

                schema[i].kv[j].type = value;
                break;
            }
            } // switch
            lua_pop(L, 1);
            j++;
        }
        MEMRES(schema[i].kv, sizeof(SchemaKV) * (j + 1));
        memset(&schema[i].kv[j], 0, sizeof(SchemaKV));
        lua_pop(L, 1);
        i++;
    }
    if (i > 0) {
        MEMRES(schema, sizeof(Schema) * (i + 1));
        schema[i].kv = NULL;
    }
    if (schema) {
        /* lua_pushlightuserdata(L, schema); */
        /* lua_setglobal(L, "SCHEMA"); */
        Schema *p = schema;
        bool *nopes = NULL;
        while(p->kv) {
            int f = p - schema;
            MEMRES(nopes, sizeof(bool) * (f + 1));
            nopes[f] = true;
            if (!database_checkTable(&engine->database, p->name)) {
                nopes[f] = false;
                SchemaKV *kv = lua_findschemaKV(schema, p->name);
                database_createTable(&engine->database, p->name, kv);
                while(!database_checkTable(&engine->database, p->name));
            }
            p++;
        }

        p = schema;

        database_enableForeignKeys(&engine->database);

        while(p->kv) {
            int f = p - schema;
            if (!nopes[f]) {
                SchemaKV *kv = lua_findschemaKV(schema, p->name);
                database_addForeignKeys(&engine->database, p->name, kv);
            }
            p++;
        }

        free(nopes);
    }
    for (int j = 0; j < i; ++j) {
        free(schema[j].kv);
    }
    free(schema);
    return 0;
}


void _insert_callback(char *name, void *data)
{
    _insert_data *ld = (_insert_data *)data;

    if (!lua_istable(ld->state, ld->index)) {
        N2DE_ERROR("not a table");
        exit(1);
    }

    lua_pushnil(ld->state);
    while(lua_next(ld->state, ld->index)) {
        char *key  = (char *)luaL_checkstring(ld->state, -2);

        if (!strcmp(key, name)) {
            int type = lua_type(ld->state, -1);
            if (type == LUA_TNUMBER) {
                int a  = (int)luaL_checknumber(ld->state, -1);
                char num[10];
                sprintf(num, "%d", a);
                strcat(ld->sql, num);
            }
            if (type == LUA_TSTRING) {
                char *a  = (char *)luaL_checkstring(ld->state, -1);
                char str[SQLITE_MAX_STRING_SIZE];
                sprintf(str, "\"%s\"", a);
                strcat(ld->sql, str);
            }
            strcat(ld->sql, ",");
        }
        lua_pop(ld->state, 1);
    }
    /* printf("sql: %s\n", ld->sql); */
}

void _update_callback(char *name, void *data)
{
    _update_data *ld = (_update_data *)data;

    if (!lua_istable(ld->state, ld->index)) {
        N2DE_ERROR("not a table");
        exit(1);
    }

    lua_pushnil(ld->state);
    while(lua_next(ld->state, ld->index)) {
        char *key  = (char *)luaL_checkstring(ld->state, -2);

        if (!strcmp(key, name)) {
            int type = lua_type(ld->state, -1);
            strcat(ld->sql, name);
            strcat(ld->sql, " = ");
            if (type == LUA_TNUMBER) {
                int a  = (int)luaL_checknumber(ld->state, -1);
                char num[10];
                sprintf(num, "%d", a);
                strcat(ld->sql, num);
            }
            if (type == LUA_TSTRING) {
                char *a  = (char *)luaL_checkstring(ld->state, -1);
                char str[SQLITE_MAX_STRING_SIZE];
                sprintf(str, "\"%s\"", a);
                strcat(ld->sql, str);
            }
            strcat(ld->sql, ",");
        }
        lua_pop(ld->state, 1);
    }
}

__declspec(dllexport) int libdatabase_insert(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *table = (char *)luaL_checkstring(L, 1);

    bool isTable = database_checkTable(&engine->database, table);

    if (!isTable) {
        N2DE_ERROR("%s is not a table", table);
        exit(1);
    }

    char sql[SQLITE_MAX_QUERY];
    char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE];

    int size = database_getColumnNames(&engine->database, table, names);
    sprintf(sql, "INSERT INTO %s (", table);
    for (int i = 1; i < size; ++i) {
        strcat(sql, names[i]);
        if (i + 1 != size) strcat(sql, ",");
    }
    strcat(sql, ") VALUES (");
    _insert_data ld = {2, L, sql};
    printf("%s\n", sql);
    printf("sql len: %d\n", strlen(sql));
    database_columns(&engine->database, table, (void *)&ld, _insert_callback);
    sql[strlen(sql) - 1] = '\0';
    strcat(sql, ")");
    database_exec(&engine->database, sql);
    return 0;
}

__declspec(dllexport) int libdatabase_update(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);

    char *table = (char *)luaL_checkstring(L, 1);
    uint8_t id = luaL_checknumber(L, 2);

    bool isTable = database_checkTable(&engine->database, table);

    if (!isTable) {
        N2DE_ERROR("%s is not a table", table);
        exit(1);
    }

    char key_values[SQLITE_MAX_QUERY];
    memset(key_values, 0, SQLITE_MAX_QUERY);
    char sql[SQLITE_MAX_QUERY];
    memset(sql, 0, SQLITE_MAX_QUERY);
    _update_data ld = {3, L, key_values};
    database_columns(&engine->database, table, (void *)&ld, _update_callback);
    key_values[strlen(key_values) - 1] = '\0';
    sprintf(sql, "UPDATE %s SET %s WHERE id = %d", table, key_values, id);
    printf("%s\n", sql);
    /* strcat(sql, ")"); */
    database_exec(&engine->database, sql);
    return 0;
}

typedef struct _getAll_data {
    lua_State *L;
    char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE];
} _getAll_data;

void _getAll_callback(sqlite3_stmt *stmt, int i, int size, void *data)
{
    _getAll_data *gad = (_getAll_data *)data;

    int id = sqlite3_column_int(stmt, 0);
    lua_pushnumber(gad->L, id);
    lua_newtable(gad->L);
    for (int j = 1; j < size; ++j){
        int type = sqlite3_column_type(stmt, j);
        lua_pushstring(gad->L, gad->names[j]);
        switch(type) {
            case SQLITE_INTEGER: {
                int f = sqlite3_column_int(stmt, j);
                lua_pushnumber(gad->L, f);
                break;
            }
            case SQLITE_TEXT: {
                char *f = (char *)sqlite3_column_text(stmt, j);
                lua_pushstring(gad->L, f);
                break;
            }
        }
        lua_settable(gad->L, -3);
    }
    lua_settable(gad->L, -3);
}

void _getFromUUID_callback(sqlite3_stmt *stmt, int i, int size, void *data)
{
    _getAll_data *gad = (_getAll_data *)data;

    int id = sqlite3_column_int(stmt, 0);
    for (int j = 1; j < size; ++j){
        int type = sqlite3_column_type(stmt, j);
        lua_pushstring(gad->L, gad->names[j]);
        switch(type) {
            case SQLITE_INTEGER: {
                int f = sqlite3_column_int(stmt, j);
                lua_pushnumber(gad->L, f);
                break;
            }
            case SQLITE_TEXT: {
                char *f = (char *)sqlite3_column_text(stmt, j);
                lua_pushstring(gad->L, f);
                break;
            }
        }
        lua_settable(gad->L, -3);
    }
}

__declspec(dllexport) int libdatabase_getAll(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *table = (char *)luaL_checkstring(L, 1);
    bool exists = database_checkTable(&engine->database, table);
    if (!exists) {
        lua_pushnil(L);
        return 1;
    }
    char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE];
    database_getColumnNames(&engine->database, table, names);

    _getAll_data data;
    data.L = L;
    memcpy(data.names, names, sizeof(names));
    lua_newtable(L);
    database_getAll(&engine->database, table, _getAll_callback, (void *)&data);
    return 1;
}

__declspec(dllexport) int libdatabase_getFromUUID(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *table = (char *)luaL_checkstring(L, 1);
    bool exists = database_checkTable(&engine->database, table);
    if (!exists) {
        lua_pushnil(L);
        return 1;
    }
    char *uuid = (char *)luaL_checkstring(L, 2);
    char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE];
    database_getColumnNames(&engine->database, table, names);

    _getAll_data data;
    data.L = L;
    memcpy(data.names, names, sizeof(names));
    lua_newtable(L);
    database_getFromUUID(&engine->database, table, _getFromUUID_callback, (void *)&data, uuid);
    return 1;
}

__declspec(dllexport) int libdatabase_getFromID(lua_State *L)
{
    Nyquist2DEngine *engine = NULL;
    LUA_GETENGINE(L, engine);
    char *table = (char *)luaL_checkstring(L, 1);
    bool exists = database_checkTable(&engine->database, table);
    if (!exists) {
        lua_pushnil(L);
        return 1;
    }
    char *id = (char *)luaL_checkstring(L, 2);
    char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE];
    database_getColumnNames(&engine->database, table, names);

    _getAll_data data;
    data.L = L;
    memcpy(data.names, names, sizeof(names));
    lua_newtable(L);
    database_getFromID(&engine->database, table, _getFromUUID_callback, (void *)&data, id);
    return 1;
}

__declspec(dllexport) luaL_Reg libdatabase[] = {
    {"init", libdatabase_init},
    {"insert", libdatabase_insert},
    {"getAll", libdatabase_getAll},
    {"getFromUUID", libdatabase_getFromUUID},
    {"getFromID", libdatabase_getFromID},
    {"update", libdatabase_update},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_extensions_libdatabase(lua_State *L)
{
    luaL_newlib(L, libdatabase);
    return 1;
}
