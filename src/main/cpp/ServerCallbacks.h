#pragma once

#include <steam/steam_api.h>
#include <steam/steam_gameserver.h>

#include <mutex>
#include <vector>

enum class SteamEventType
{
    ServersConnected,
    AuthTicketValid,
    AuthTicketInvalid
};

struct SteamEvent
{
    SteamEventType type;
    uint64 steamId; // only set for auth events
};

// Steam callbacks only queue events; they are delivered to Java from RunCallbacks.
class ServerCallbacks
{
public:
    ServerCallbacks();

    // Returns and clears all queued events.
    std::vector<SteamEvent> Drain();

private:
    void Push(const SteamEvent &event);

    std::mutex mutex;
    std::vector<SteamEvent> events;
    STEAM_GAMESERVER_CALLBACK_MANUAL(ServerCallbacks, OnConnected, SteamServersConnected_t, m_Connected);
    STEAM_GAMESERVER_CALLBACK_MANUAL(ServerCallbacks, OnValidate, ValidateAuthTicketResponse_t, m_Validate);
};
