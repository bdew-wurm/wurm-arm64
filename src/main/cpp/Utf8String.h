#pragma once

#include <jni.h>
#include <steam/steamclientpublic.h>

// RAII wrapper for the UTF-8 characters of a jstring; null strings yield "".
class Utf8String
{
public:
    Utf8String(JNIEnv *env, jstring str) : env(env), str(str), chars(str ? env->GetStringUTFChars(str, nullptr) : nullptr) {}
    ~Utf8String()
    {
        if (chars)
            env->ReleaseStringUTFChars(str, chars);
    }
    Utf8String(const Utf8String &) = delete;
    Utf8String &operator=(const Utf8String &) = delete;
    const char *c_str() const { return chars ? chars : ""; }

private:
    JNIEnv *env;
    jstring str;
    const char *chars;
};

// Parses a decimal 64-bit Steam ID passed from Java as a string.
CSteamID ParseSteamId(JNIEnv *env, jstring id);
