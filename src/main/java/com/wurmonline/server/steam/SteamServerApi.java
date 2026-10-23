package com.wurmonline.server.steam;

public class SteamServerApi {
    private final SteamHandler steamHandler;

    /**
     * Creates the JNI bridge and records the handler that receives Steam callbacks.
     * 
     * @param steamHandler Server-side handler that processes Steam callbacks.
     */
    public SteamServerApi(final SteamHandler steamHandler) {
        this.steamHandler = steamHandler;
    }

    /**
     * Creates the native callback object used to forward Steam server events to
     * this instance.
     */
    public native void CreateCallback();

    /** Destroys the native callback object created by {@link #CreateCallback()}. */
    public native void DeleteCallback();

    /**
     * Processes pending Steam game-server events, including connection and
     * authentication callbacks.
     */
    public native void SteamGameServer_RunCallbacks();

    /**
     * Initializes the Steam game server.
     * The ports, server mode, and version are supplied by
     * {@code SteamHandler.createServer}.
     * 
     * @param serverIp      IP address used for the server; the current caller
     *                      supplies zero.
     * @param steamPort     Steam server port (the current caller supplies 8766).
     * @param gamePort      Port on which the game server accepts connections.
     * @param queryPort     Port used for Steam server queries.
     * @param serverMode    Steam server mode; the caller selects secure
     *                      authentication online
     *                      and no authentication when running offline.
     * @param versionString Server version sent to Steam.
     */
    public native boolean SteamGameServer_Init(final long serverIp, final short steamPort, final short gamePort,
            final short queryPort, final int serverMode, final String versionString);

    /**
     * Sets the server's mod directory as advertised to Steam.
     * 
     * @param modDirectory Server mod directory.
     */
    public native void SetModDir(final String modDirectory);

    /**
     * Sets whether this server is a dedicated server.
     * 
     * @param isDedicatedServer True for this dedicated server.
     */
    public native void SetDedicatedServer(final boolean isDedicatedServer);

    /**
     * Sets the product name advertised for this game server.
     * 
     * @param productName Product name supplied by the server configuration.
     */
    public native void SetProduct(final String productName);

    /**
     * Sets the game description advertised for this game server.
     * 
     * @param gameDescription Description supplied by the server configuration.
     */
    public native void SetGameDescription(final String gameDescription);

    /**
     * Sets the tags used to describe and classify this server in Steam.
     * 
     * @param gameTags Version and server-status tags.
     */
    public native void SetGameTags(final String gameTags);

    /** Logs the game server into Steam without account credentials. */
    public native void LogOnAnonymous();

    /**
     * Enables or disables heartbeat updates that keep the server listed by Steam.
     * 
     * @param enabled True to send heartbeats; false to stop them.
     */
    public native void EnableHeartbeats(final boolean enabled);

    /** Logs the game server out of Steam. */
    public native void LogOff();

    /** Shuts down the Steam game-server session. */
    public native void SteamGameServer_Shutdown();

    /**
     * Sets the maximum number of players advertised for this server.
     * 
     * @param maximumPlayerCount Configured player limit.
     */
    public native void setMaxPlayerCount(final int maximumPlayerCount);

    /**
     * Sets whether Steam should list this server as password-protected.
     * 
     * @param passwordProtected True when the server has a password.
     */
    public native void SetPasswordProtected(final boolean passwordProtected);

    /**
     * Sets the server name advertised to Steam.
     * 
     * @param serverName Name from the local server configuration.
     */
    public native void SetServerName(final String serverName);

    /**
     * Sets the number of bots advertised for this server.
     * 
     * @param botCount Configured bot count.
     */
    public native void SetBotCount(final int botCount);

    /**
     * Sets the map name advertised for this server.
     * 
     * @param mapName Map name from the local server configuration.
     */
    public native void SetMapName(final String mapName);

    /**
     * Begins Steam authentication for the given player using their ticket and its
     * supplied length.
     * The result is interpreted by {@code SteamHandler} as the authentication
     * result code.
     * 
     * @param steamId              Player's Steam ID as a string.
     * @param authenticationTicket Authentication ticket bytes received from the
     *                             login client.
     * @param ticketLength         Ticket length value received separately from the
     *                             login client.
     */
    public native int BeginAuthSession(final String steamId, final byte[] authenticationTicket,
            final long ticketLength);

    /**
     * Ends the Steam authentication session for the given player.
     * 
     * @param steamId Player's Steam ID as a string.
     */
    public native void EndAuthSession(final String steamId);

    /**
     * Handles Steam's server-connected callback by configuring the server's listing
     * details.
     */
    public void OnSteamServersConnected() {
        this.steamHandler.onSteamConnected();
    }

    /**
     * Forwards Steam's ticket-validation result for a player to the server handler.
     * 
     * @param steamId       Player's Steam ID as a string.
     * @param authenticated Whether Steam accepted the player's authentication
     *                      ticket.
     */
    public void OnValidateAuthTicketResponse(final String steamId, final boolean authenticated) {
        this.steamHandler.onValidateAuthTicketResponse(steamId, authenticated);
    }

    static {
        System.loadLibrary("SteamServerJni");
    }
}
