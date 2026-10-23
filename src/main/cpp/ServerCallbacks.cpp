#include "ServerCallbacks.h"
#include "DebugLog.h"

ServerCallbacks::ServerCallbacks()
{
    m_Connected.Register(this, &ServerCallbacks::OnConnected);
    m_Validate.Register(this, &ServerCallbacks::OnValidate);
}

std::vector<SteamEvent> ServerCallbacks::Drain()
{
    std::lock_guard<std::mutex> lock(mutex);
    std::vector<SteamEvent> out;
    out.swap(events);
    return out;
}

void ServerCallbacks::Push(const SteamEvent &event)
{
    std::lock_guard<std::mutex> lock(mutex);
    events.push_back(event);
}

void ServerCallbacks::OnConnected(SteamServersConnected_t *)
{
    DEBUG_LOG("OnConnected");
    Push({SteamEventType::ServersConnected, 0});
}

void ServerCallbacks::OnValidate(ValidateAuthTicketResponse_t *p)
{
    bool ok = p->m_eAuthSessionResponse == k_EAuthSessionResponseOK;
    uint64 steamId = p->m_SteamID.ConvertToUint64();
    DEBUG_LOG("OnValidate(steamId=%llu, response=%d, authenticated=%d)", (unsigned long long)steamId, (int)p->m_eAuthSessionResponse, (int)ok);
    Push({ok ? SteamEventType::AuthTicketValid : SteamEventType::AuthTicketInvalid, steamId});
}
