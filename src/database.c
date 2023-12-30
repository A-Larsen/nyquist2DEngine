#include <stdio.h>

#include "N2DE/database.h"

// TODO:
// Make it possible to create schema in lua you can use with sql

int main(void)
{
    SchemaKV things_schema[] = {
        {"id", SQLITE_INTEGER},
        {"type", SQLITE_TEXT},
        {"\0", 0},
    };

    SchemaKV people_schema[] = {
        {"id", SQLITE_INTEGER},
        {"name", SQLITE_TEXT},
        {"age", SQLITE_INTEGER},
        {"\0", 0},
    };

    SchemaKV a_schema[] = {
        {"a", SQLITE_INTEGER},
        {"b", SQLITE_TEXT},
        {"c", SQLITE_INTEGER},
        {"d", SQLITE_INTEGER},
        {"e", SQLITE_TEXT},
        {"f", SQLITE_TEXT},
        {"\0", 0},
    };

    Database db;
    database_init(&db, "./.res/test.db");

    bool isTable = database_checkTable(&db, "things");

    if (!isTable) {
        database_createTable(&db, "things", things_schema);
        while(!database_checkTable(&db, "things"));
        database_insert(&db, "things", things_schema, 321, "catdog");
        database_insert(&db, "things", things_schema, 1, "air");
    }


    /* database_vdataAddInt(&db, 71); */
    /* database_vdataAddText(&db, "sup"); */
    /* database_vdataAddInt(&db, 4); */
    /* database_vdataAddInt(&db, 81); */
    /* database_vdataAddText(&db, "no"); */
    /* database_vdataAddText(&db, "u"); */

    /* database_vinsert(&db, "people", a_schema); */

    /* printf("things -> isTable: %d\n", isTable); */

    isTable = database_checkTable(&db, "birds");
    printf("birds -> isTable: %d\n", isTable);

    database_getManyInt(&db, "things", "id");

    for (int i = 0; i < db.data_size; ++i) {
        printf("%d\n", db.data_int[i]);
    }

    database_getManyText(&db, "things", "type");

    for (int i = 0; i < db.data_size; ++i) {
        printf("%s\n", db.data_text[i]);
    }


    database_quit(&db);
}
