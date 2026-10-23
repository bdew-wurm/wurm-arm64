#include "Utf8String.h"

#include <cstdlib>

CSteamID ParseSteamId(JNIEnv *env, jstring id)
{
    Utf8String tmp(env, id);
    return CSteamID((uint64)strtoull(tmp.c_str(), nullptr, 10));
}
