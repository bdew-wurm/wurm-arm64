#pragma once

#include <cstdio>

// Set to 0 (or pass -DSTEAM_JNI_DEBUG=0) to compile out all debug prints.
#ifndef STEAM_JNI_DEBUG
#define STEAM_JNI_DEBUG 1
#endif

#if STEAM_JNI_DEBUG
#define DEBUG_LOG(...)                    \
    do                                    \
    {                                     \
        printf("[SteamServerJni] ");      \
        printf(__VA_ARGS__);              \
        printf("\n");                     \
        fflush(stdout);                   \
    } while (0)
#else
#define DEBUG_LOG(...) \
    do                 \
    {                  \
    } while (0)
#endif
