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

#ifndef N2DE_DATABASE_H_
#define N2DE_DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "../sqlite3/sqlite3.h"
#include "memory.h"
#include "files.h"
/* #include "uuid.h" */

#define SQLITE_FOREIGN 6
#define SQLITE_INTEGER_UNIQUE 7
#define SQLITE_TEXT_UNIQUE 8

#define SQLITE_MAX_STRING_SIZE 255
#define SQLITE_MAX_ROW_COUNT 20
#define SQLITE_MAX_QUERY 1024

#define DATABASE_BADSTEP(db, ret) \
    if((ret != SQLITE_DONE) && (ret != SQLITE_ROW)) { \
        N2DE_ERROR( "could not create table\n%s", \
                sqlite3_errmsg(db->sql_db)); \
        exit(1); \
    }

/* because all N2DE headers that are a direct child of the N2DE folder
 * should be able to be used outside of the use of the engine and be modular
 * I will no include respath here, but will later on
 */

typedef const char * sql_t;
typedef void (*db_columns_cb) (char *name, void *data);
typedef void (*db_getall_cb) (sqlite3_stmt *stmt, int i, int size, void *data);

typedef struct _Database {
    sqlite3 *sql_db;
    int type;
    int *data_int;
    char **data_text;
    uint16_t data_size;
} Database;

typedef struct _SchemaKV {
    char name[50];
    char foreign[50];
    int type;
} SchemaKV;

typedef struct _Schema {
    char name[50];
    SchemaKV *kv;
} Schema;

static void _database_freeInt(Database *db)
{
    if (db->data_size > 0 && db->type == SQLITE_INTEGER) {
        free(db->data_int);
    }
}

static void _database_freeText(Database *db)
{
    if (db->data_size > 0 && db->type == SQLITE_TEXT) {
        for (int i = 0; i < db->data_size; ++i) {
            free(db->data_text[i]);
        }
        free(db->data_text);
    }
}

static void database_prepare(Database *db, sqlite3_stmt **stmt, sql_t sql)
{
    if (sqlite3_prepare_v2(db->sql_db, sql, -1, stmt, 0) != SQLITE_OK) {
        N2DE_ERROR("Faild to fetch data\n%s\n%s", sqlite3_errmsg(db->sql_db),  sql);
        exit(1);
    }
}

void database_insert(Database *db, char * table,  SchemaKV schema[],
                     ...)
{
    sqlite3_stmt *stmt = NULL;
    va_list args;
    va_start(args, schema);
    char sql[255];
    sprintf(sql, "INSERT INTO %s VALUES (", table);
    SchemaKV *p = schema;
    while(p->name[0]) {

        switch (p->type) {
        case SQLITE_INTEGER: {
            int a = va_arg(args, int);
            char str[10];
            sprintf(str, "%d", a);
            strcat(sql, str);
            break;
        }
        case SQLITE_TEXT: {
            const char *a = va_arg(args, const char *);
            char str[10];
            sprintf(str, "\"%s\"", a);
            strcat(sql, str);
            break;
        }
        // Was going to add blob data but decided to make a higher level of
        // abstraction for things like blob. I could probably make a seperate
        // (probably not even sql) database for things like make data. Just
        // write a struct to a file with non-dynamic/non-pointer data. 
        }

        p++;

        if (p->name[0]) strcat(sql, ",");
    }

    printf("inserting\n%s\n", sql);
    strcat(sql, ")");

    database_prepare(db, &stmt, sql);
    int ret = sqlite3_step(stmt);

    DATABASE_BADSTEP(db, ret);

    sqlite3_finalize(stmt);
    va_end(args);
}

void database_columns(Database *db, char *table, void *data, db_columns_cb callback)
{
    sqlite3_stmt *stmt;
    char sql[255];
    sprintf(sql, "SELECT name FROM PRAGMA_TABLE_INFO('%s')", table);
    database_prepare(db, &stmt, sql);
    int ret;
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *name = (char *)sqlite3_column_text(stmt, 0);
        printf("colums name: %s\n",name);
        callback(name, data);
    }
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);

}

void database_getManyInt(Database *db, char *table, char *row)
{
    sqlite3_stmt *stmt;
    _database_freeInt(db);

    db->type = SQLITE_INTEGER;

    int i = 0;
    char sql[255];
    sprintf(sql, "SELECT %s FROM %s", row, table);
    database_prepare(db, &stmt, sql);
    int ret;
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        int type = sqlite3_column_type(stmt, 0);

        if (type != SQLITE_INTEGER) {
           N2DE_ERROR("data is not an integer");
           exit(1);
        }

        int ret = sqlite3_column_int(stmt, 0);
        MEMRES(db->data_int, sizeof(int) * (i + 1));
        (db->data_int)[i] = ret;
        i++;
    }
    DATABASE_BADSTEP(db, ret);

    db->data_size = i;
    sqlite3_finalize(stmt);
}

void database_getManyText(Database *db, char *table, char *row)
{
    sqlite3_stmt *stmt;
    _database_freeText(db);

    db->type = SQLITE_TEXT;

    int i = 0;
    char sql[255];
    sprintf(sql, "SELECT %s FROM %s", row, table);
    database_prepare(db, &stmt, sql);

    int ret;
    while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        int type = sqlite3_column_type(stmt, 0);

        if (type != SQLITE_TEXT) {
           N2DE_ERROR("data is not text");
           exit(1);
        }
        const char *ret = (char *)sqlite3_column_text(stmt, 0);

        int len = strlen(ret);

        int vsize = (sizeof(char) * len) + 1;

        MEMRES(db->data_text, sizeof(char *) * (i + 1));

        (db->data_text)[i] = malloc(vsize);
        memset((db->data_text)[i], 0, vsize);
        memcpy((db->data_text)[i], ret, len);
        i++;
    }

    DATABASE_BADSTEP(db, ret);

    db->data_size = i;
    sqlite3_finalize(stmt);
}

void database_init(Database *db, char *path)
{
    memset(db, 0, sizeof(Database)); // should be in database_init
    db->data_text = NULL;
    db->data_int = NULL;

    bool exists = access(path, F_OK) == 0;
    if (!exists) {
        touch(path);
    }
    if (sqlite3_open_v2(path, &db->sql_db, SQLITE_OPEN_READWRITE, NULL) !=
        SQLITE_OK) {
        N2DE_ERROR("Can't open database:\n%s", sqlite3_errmsg(db->sql_db));
        exit(1);
    }
}

bool database_checkTable(Database *db, char *table) {
    sqlite3_stmt *stmt;
    database_prepare(db, &stmt,
            "SELECT 1 FROM sqlite_master where type='table' and name=?");

    sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);

    bool found;

    if (rc == SQLITE_ROW)
        found = true;
    else if (rc == SQLITE_DONE)
        found = false;
    else {
        N2DE_ERROR("could not step\n%s", sqlite3_errmsg(db->sql_db));
        sqlite3_finalize(stmt);
        exit(1);
    }

    sqlite3_finalize(stmt);
    return found;
}


void database_createTable(Database *db, char * table, SchemaKV *schema)
{
    sqlite3_stmt *stmt;
    char sql[800];
    sprintf(sql, "CREATE TABLE %s "
            "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE", table);

    SchemaKV *p = schema;
    bool hasUniques = false;
    while(p->name[0]) {
        if (p->type == SQLITE_FOREIGN) {
            p++;
            continue;
        }
        strcat(sql, ",");
        /* if ((p - schema) == 0) { */
        /*     strcat(sql, ","); */
        /* } */
        strcat(sql, p->name);
        switch (p->type) {
        /* case SQLITE_INTEGER_UNIQUE: */ 
        /* case SQLITE_TEXT_UNIQUE: { */
        /*     hasUniques = true; */
        /*     break; */
        /* } */
        case SQLITE_INTEGER: {
             strcat(sql, " INTEGER"); 
             break;
        }
        case SQLITE_TEXT: {
            strcat(sql, " VARCHAR(255)");
            break;
        }
        case SQLITE_BLOB: {
            strcat(sql, " BLOB");
            break;
        }
        case SQLITE_INTEGER_UNIQUE: {
            strcat(sql, " INTEGER");
            break;
        }
        case SQLITE_TEXT_UNIQUE: {
            strcat(sql, " VARCHAR(255)");
            break;
        }
        } // switch
        switch (p->type) {
        case SQLITE_INTEGER_UNIQUE: 
        case SQLITE_TEXT_UNIQUE: 
            hasUniques = true;
            break;
       
        } // switch

        p++;

        /* if (p->name[0] && (p->type != SQLITE_FOREIGN)) strcat(sql, ","); */
        /* if (p->type != SQLITE_FOREIGN) strcat(sql, ","); */
        /* if (p->name[0]) strcat(sql, ","); */
        /* if (!(p->name[0] || p->type != SQLITE_FOREIGN)) strcat(sql, ","); */
    }
    if (hasUniques) {
        p = schema;
        strcat(sql, ", UNIQUE(");
        bool before_found = false;
        while(p->name[0]) {
            if (p->type == SQLITE_FOREIGN) {
                p++;
                continue;
            }

            switch (p->type) {
            case SQLITE_TEXT_UNIQUE: 
            case SQLITE_INTEGER_UNIQUE: {
                if (before_found) {
                    strcat(sql, ",");
                    before_found = false;
                }
                break;
            }

            } // switch

            switch (p->type) {
            case SQLITE_TEXT_UNIQUE: {
                strcat(sql, p->name);
                break;
            }
            case SQLITE_INTEGER_UNIQUE: {
                strcat(sql, p->name);
                break;
            }
            } // switch

            switch (p->type) {
            case SQLITE_TEXT_UNIQUE: 
            case SQLITE_INTEGER_UNIQUE: {
                before_found = true;
                break;
            }
            } // switch

            p++;

        }
        strcat(sql, ")");
    }

    strcat(sql, ")");

    database_prepare(db, &stmt, sql);
    int ret = sqlite3_step(stmt);

    DATABASE_BADSTEP(db, ret);

    sqlite3_finalize(stmt);
}

void database_enableForeignKeys(Database *db)
{
    sqlite3_stmt *stmt = NULL;
    database_prepare(db, &stmt, "PRAGMA foreign_keys = ON");
    int ret = sqlite3_step(stmt);
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
}

void database_addForeignKeys(Database *db, char * table, SchemaKV *kv)
{

    SchemaKV *p = kv;
    bool found = false;
    while(p->name[0]) {
        if (p->type == SQLITE_FOREIGN) {
            found = true;
        }
        p++;
    }
    if (!found) return;

    p = kv;
    while(p->name[0]) {
        sqlite3_stmt *stmt = NULL;
        char sql[500];
        sprintf(sql, "ALTER TABLE %s ", table);

        if (p->type != SQLITE_FOREIGN) {
            p++;
            continue;
        }
        char str[255];
        sprintf(str, "ADD COLUMN %s INT REFERENCES %s(id)", p->name,
                p->foreign);

        strcat(sql, str);

        p++;

        database_prepare(db, &stmt, sql);
        int ret = sqlite3_step(stmt);
        DATABASE_BADSTEP(db, ret);
        sqlite3_finalize(stmt);
    }
}

void database_exec(Database *db, char *sql)
{
    sqlite3_stmt *stmt;
    database_prepare(db, &stmt, sql);
    int ret = sqlite3_step(stmt);

    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
}


// must allocate enough space for names
int database_getColumnNames(Database *db, char *table, char names[SQLITE_MAX_ROW_COUNT][SQLITE_MAX_STRING_SIZE])
{
    char sql[255];
    sqlite3_stmt *stmt = NULL;
    sprintf(sql, "SELECT name FROM PRAGMA_TABLE_INFO('%s')", table);
    database_prepare(db, &stmt, sql);
    int i = 0;
    int ret;
    while((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        char *name = (char *)sqlite3_column_text(stmt, 0);
        memset(names[i], 0, 255);
        memcpy(names[i], name, strlen(name));
        i++;
    }
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
    return i;
}

void database_getAll(Database *db, char *table, db_getall_cb callback,
                     void *data)
{
    char sql[255];
    sqlite3_stmt *stmt = NULL;
    stmt = NULL;
    memset(sql, 0, 255);
    sprintf(sql, "SELECT * FROM %s", table);
    database_prepare(db, &stmt, sql);
    int i = 0;
    int ret;
    while((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        int size = sqlite3_column_count(stmt);
        callback(stmt, i, size, data);
        i++;
    }
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
}

void database_getFromUUID(Database *db, char *table, db_getall_cb callback,
                     void *data, char *uuid)
{
    char sql[500];
    sqlite3_stmt *stmt = NULL;
    stmt = NULL;
    memset(sql, 0, 500);
    /* sprintf(sql, "SELECT * FROM %s", table); */
    sprintf(sql, "SELECT * FROM %s WHERE uuid = '%s'", table, uuid);
    database_prepare(db, &stmt, sql);
    int i = 0;
    int ret;
    while((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        int size = sqlite3_column_count(stmt);
        callback(stmt, i, size, data);
        i++;
    }
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
}

void database_getFromID(Database *db, char *table, db_getall_cb callback,
                     void *data, char *id)
{
    char sql[500];
    sqlite3_stmt *stmt = NULL;
    stmt = NULL;
    memset(sql, 0, 500);
    /* sprintf(sql, "SELECT * FROM %s", table); */
    sprintf(sql, "SELECT * FROM %s WHERE id = '%s'", table, id);
    database_prepare(db, &stmt, sql);
    int i = 0;
    int ret;
    while((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
        int size = sqlite3_column_count(stmt);
        callback(stmt, i, size, data);
        i++;
    }
    DATABASE_BADSTEP(db, ret);
    sqlite3_finalize(stmt);
}

void database_quit(Database *db)
{
    _database_freeText(db);
    _database_freeInt(db);
    sqlite3_close(db->sql_db);
}

#endif // N2DE_DATABASE_H_
