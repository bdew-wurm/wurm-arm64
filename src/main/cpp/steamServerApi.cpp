#include "com_wurmonline_server_steam_SteamServerApi.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *, void *)
{
    printf("SteamServerJni custom implementation loaded\n");
    fflush(stdout);
    return JNI_VERSION_1_8;
}

// Creates the callback object that forwards Steam server events to this instance.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_CreateCallback(JNIEnv *env, jobject self)
{
}

// Destroys the callback object created by CreateCallback.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_DeleteCallback(JNIEnv *env, jobject self)
{
}

// Processes pending Steam game-server events, including connection and authentication callbacks.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1RunCallbacks(JNIEnv *env, jobject self)
{
}

// Initializes the game server with its IP, Steam/game/query ports, server mode, and version.
JNIEXPORT jboolean JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1Init(JNIEnv *env, jobject self, jlong serverIp, jshort steamPort, jshort gamePort, jshort queryPort, jint serverMode, jstring versionString)
{
    return JNI_FALSE;
}

// Sets the mod directory advertised to Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetModDir(JNIEnv *env, jobject self, jstring modDirectory)
{
}

// Sets whether this server is dedicated.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetDedicatedServer(JNIEnv *env, jobject self, jboolean isDedicatedServer)
{
}

// Sets the product name advertised for this game server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetProduct(JNIEnv *env, jobject self, jstring productName)
{
}

// Sets the game description advertised for this game server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetGameDescription(JNIEnv *env, jobject self, jstring gameDescription)
{
}

// Sets the tags used to describe and classify this server in Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetGameTags(JNIEnv *env, jobject self, jstring gameTags)
{
}

// Logs the game server into Steam without account credentials.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_LogOnAnonymous(JNIEnv *env, jobject self)
{
}

// Enables or disables heartbeat updates that keep the server listed by Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_EnableHeartbeats(JNIEnv *env, jobject self, jboolean enabled)
{
}

// Logs the game server out of Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_LogOff(JNIEnv *env, jobject self)
{
}

// Shuts down the Steam game-server session.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SteamGameServer_1Shutdown(JNIEnv *env, jobject self)
{
}

// Sets the maximum number of players advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_setMaxPlayerCount(JNIEnv *env, jobject self, jint maximumPlayerCount)
{
}

// Sets whether Steam should list this server as password-protected.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetPasswordProtected(JNIEnv *env, jobject self, jboolean passwordProtected)
{
}

// Sets the server name advertised to Steam.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetServerName(JNIEnv *env, jobject self, jstring serverName)
{
}

// Sets the number of bots advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetBotCount(JNIEnv *env, jobject self, jint botCount)
{
}

// Sets the map name advertised for this server.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_SetMapName(JNIEnv *env, jobject self, jstring mapName)
{
}

// Begins Steam authentication with the player's Steam ID, ticket bytes, and supplied ticket length.
JNIEXPORT jint JNICALL Java_com_wurmonline_server_steam_SteamServerApi_BeginAuthSession(JNIEnv *env, jobject self, jstring steamId, jbyteArray authenticationTicket, jlong ticketLength)
{
    return 0;
}

// Ends the Steam authentication session for the given player's Steam ID.
JNIEXPORT void JNICALL Java_com_wurmonline_server_steam_SteamServerApi_EndAuthSession(JNIEnv *env, jobject self, jstring steamId)
{
}