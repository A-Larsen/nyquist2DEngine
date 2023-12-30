#ifndef N2DE_MEMORY_H_
#define N2DE_MEMORY_H_

#include "error.h"

#define MEMRES(p, size) \
    p = realloc(p, size); \
    if (!p) { \
        N2DE_ERROR("could not allocate memory"); \
        exit(1); \
    }

#endif // N2DE_MEMORY_H_
