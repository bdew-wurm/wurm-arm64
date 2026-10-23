#include "com_wurmonline_server_steam_SteamServerApi.h"

#include "Utf8String.h"
#include "ServerCallbacks.h"
#include "DebugLog.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

namespace
{
    ServerCallbacks *callbacks = nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *, void *)
{
    printf("SteamServerJni custom implementation loaded\n");
    fflush(stdout);
    return JNI_VERSION_1_8;
}

// Creates the callback object that forwards Steam server events to this instance.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_CreateCallback(JNIEnv *env, jobject self)
{
    DEBUG_LOG("CreateCallback");
    if (!callbacks)
        callbacks = new ServerCallbacks();
}

// Destroys the callback object created by CreateCallback.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_DeleteCallback(JNIEnv *env, jobject self)
{
    DEBUG_LOG("DeleteCallback");
    delete callbacks;
    callbacks = nullptr;
}

// Processes pending Steam game-server events, including connection and authentication callbacks.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1RunCallbacks(JNIEnv *env, jobject self)
{
    ::SteamGameServer_RunCallbacks();
    if (!callbacks)
        return;
    std::vector<SteamEvent> events = callbacks->Drain();
    if (events.empty())
        return;
    DEBUG_LOG("SteamGameServer_RunCallbacks: dispatching %d event(s)", (int)events.size());
    jclass cls = env->GetObjectClass(self);
    jmethodID onConnected = env->GetMethodID(cls, "OnSteamServersConnected", "()V");
    jmethodID onValidate = env->GetMethodID(cls, "OnValidateAuthTicketResponse", "(Ljava/lang/String;Z)V");
    for (const SteamEvent &e : events)
    {
        if (e.type == SteamEventType::ServersConnected)
        {
            env->CallVoidMethod(self, onConnected);
        }
        else
        {
            jstring id = env->NewStringUTF(std::to_string(e.steamId).c_str());
            env->CallVoidMethod(self, onValidate, id, e.type == SteamEventType::AuthTicketValid ? JNI_TRUE : JNI_FALSE);
            env->DeleteLocalRef(id);
        }
        if (env->ExceptionCheck())
            env->ExceptionDescribe();
    }
    env->DeleteLocalRef(cls);
}

// Initializes the game server with its IP, Steam/game/query ports, server mode, and version.
JNIEXPORT jboolean JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1Init(JNIEnv *env, jobject self, jlong serverIp, jshort steamPort, jshort gamePort, jshort queryPort, jint serverMode, jstring versionString)
{
    DEBUG_LOG("SteamGameServer_Init(ip=%lld, steamPort=%d, gamePort=%d, queryPort=%d, mode=%d)", (long long)serverIp, (int)steamPort, (int)gamePort, (int)queryPort, (int)serverMode);
    Utf8String version(env, versionString);
    // steamPort is unused: the SDK no longer takes a separate Steam port.
    bool ok = ::SteamGameServer_Init((uint32)serverIp, (uint16)gamePort, (uint16)queryPort, (EServerMode)serverMode, version.c_str());
    DEBUG_LOG("SteamGameServer_Init version=%s result=%d", version.c_str(), (int)ok);
    return ok ? JNI_TRUE : JNI_FALSE;
}

// Sets the mod directory advertised to Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetModDir(JNIEnv *env, jobject self, jstring modDirectory)
{
    Utf8String tmp(env, modDirectory);
    DEBUG_LOG("SetModDir(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetModDir(tmp.c_str());
}

// Sets whether this server is dedicated.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetDedicatedServer(JNIEnv *env, jobject self, jboolean isDedicatedServer)
{
    DEBUG_LOG("SetDedicatedServer(%d)", (int)isDedicatedServer);
    if (SteamGameServer())
        SteamGameServer()->SetDedicatedServer((bool)isDedicatedServer);
}

// Sets the product name advertised for this game server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetProduct(JNIEnv *env, jobject self, jstring productName)
{
    Utf8String tmp(env, productName);
    DEBUG_LOG("SetProduct(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetProduct(tmp.c_str());
}

// Sets the game description advertised for this game server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetGameDescription(JNIEnv *env, jobject self, jstring gameDescription)
{
    Utf8String tmp(env, gameDescription);
    DEBUG_LOG("SetGameDescription(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetGameDescription(tmp.c_str());
}

// Sets the tags used to describe and classify this server in Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetGameTags(JNIEnv *env, jobject self, jstring gameTags)
{
    Utf8String tmp(env, gameTags);
    DEBUG_LOG("SetGameTags(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetGameTags(tmp.c_str());
}

// Logs the game server into Steam without account credentials.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_LogOnAnonymous(JNIEnv *env, jobject self)
{
    DEBUG_LOG("LogOnAnonymous");
    if (SteamGameServer())
        SteamGameServer()->LogOnAnonymous();
}

// Enables or disables heartbeat updates that keep the server listed by Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_EnableHeartbeats(JNIEnv *env, jobject self, jboolean enabled)
{
    DEBUG_LOG("EnableHeartbeats(%d)", (int)enabled);
    if (SteamGameServer())
        SteamGameServer()->SetAdvertiseServerActive((bool)enabled);
}

// Logs the game server out of Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_LogOff(JNIEnv *env, jobject self)
{
    DEBUG_LOG("LogOff");
    if (SteamGameServer())
        SteamGameServer()->LogOff();
}

// Shuts down the Steam game-server session.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1Shutdown(JNIEnv *env, jobject self)
{
    DEBUG_LOG("SteamGameServer_Shutdown");
    ::SteamGameServer_Shutdown();
}

// Sets the maximum number of players advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_setMaxPlayerCount(JNIEnv *env, jobject self, jint maximumPlayerCount)
{
    DEBUG_LOG("setMaxPlayerCount(%d)", (int)maximumPlayerCount);
    if (SteamGameServer())
        SteamGameServer()->SetMaxPlayerCount((int)maximumPlayerCount);
}

// Sets whether Steam should list this server as password-protected.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetPasswordProtected(JNIEnv *env, jobject self, jboolean passwordProtected)
{
    DEBUG_LOG("SetPasswordProtected(%d)", (int)passwordProtected);
    if (SteamGameServer())
        SteamGameServer()->SetPasswordProtected((bool)passwordProtected);
}

// Sets the server name advertised to Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetServerName(JNIEnv *env, jobject self, jstring serverName)
{
    Utf8String tmp(env, serverName);
    DEBUG_LOG("SetServerName(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetServerName(tmp.c_str());
}

// Sets the number of bots advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetBotCount(JNIEnv *env, jobject self, jint botCount)
{
    DEBUG_LOG("SetBotCount(%d)", (int)botCount);
    if (SteamGameServer())
        SteamGameServer()->SetBotPlayerCount((int)botCount);
}

// Sets the map name advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetMapName(JNIEnv *env, jobject self, jstring mapName)
{
    Utf8String tmp(env, mapName);
    DEBUG_LOG("SetMapName(%s)", tmp.c_str());
    if (!SteamGameServer())
        return;
    SteamGameServer()->SetMapName(tmp.c_str());
}

// Begins Steam authentication with the player's Steam ID, ticket bytes, and supplied ticket length.
JNIEXPORT jint JNICALL Java_com_wurmonline_server_steam_SteamServerApi_BeginAuthSession(JNIEnv *env, jobject self, jstring steamId, jbyteArray authenticationTicket, jlong ticketLength)
{
    Utf8String tmp(env, steamId);
    DEBUG_LOG("BeginAuthSession(%s, ticketLength=%lld)", tmp.c_str(), (long long)ticketLength);
    if (!SteamGameServer() || !authenticationTicket)
        return k_EBeginAuthSessionResultInvalidTicket;
    CSteamID id = ParseSteamId(env, steamId);
    jsize available = env->GetArrayLength(authenticationTicket);
    jsize len = (jsize)std::min<jlong>(ticketLength < 0 ? 0 : ticketLength, available);
    jbyte *data = env->GetByteArrayElements(authenticationTicket, nullptr);
    if (!data)
        return k_EBeginAuthSessionResultInvalidTicket;
    EBeginAuthSessionResult result = SteamGameServer()->BeginAuthSession(data, (int)len, id);
    env->ReleaseByteArrayElements(authenticationTicket, data, JNI_ABORT);
    DEBUG_LOG("BeginAuthSession result=%d", (int)result);
    return (jint)result;
}

// Ends the Steam authentication session for the given player's Steam ID.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_EndAuthSession(JNIEnv *env, jobject self, jstring steamId)
{
    Utf8String tmp(env, steamId);
    DEBUG_LOG("EndAuthSession(%s)", tmp.c_str());
    if (SteamGameServer())
        SteamGameServer()->EndAuthSession(ParseSteamId(env, steamId));
}
