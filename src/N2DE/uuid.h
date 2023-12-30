#ifndef N2DE_UUID_H_
#define N2DE_UUID_H_

#if defined (_WIN32)

/*
// rcpdce.h and Rpcrt4.lib is for uuid!
#include <rpcdce.h>
#pragma comment(lib, "Rpcrt4.lib" )

#define BIN_UUID_T UUID
#define UUID_CREATE(binuuid, uuid) \
    UuidCreate(&binuuid); \
    UuidToString(&binuuid, &uuid);

#define UUID_FREE(uuid) free(uuid)
*/

#else
#include <inttypes.h>

#if defined(STEAMDECK)
#include <linux/uuid.h>
#define BIN_UUID_T guid_t
#else
#include <uuid/uuid.h>
#define BIN_UUID_T uuid_t
#endif


#define UUID_CREATE(binuuid, uuid)\
    uuid_generate_random(binuuid); \
    uuid = malloc(36);  \
    uuid_unparse_upper(binuuid, uuid);

#define UUID_FREE(uuid) free(uuid)

#endif

#endif // N2DE_UUID_H_
