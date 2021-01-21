#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <stdlib.h>
#include <string.h>

#define FATAL(...) \
    do { \
    std::cout << __VA_ARGS__ << std::endl; \
    exit(1); \
    } while (0)

#endif // COMMON_H
