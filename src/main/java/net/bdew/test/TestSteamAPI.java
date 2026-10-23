package net.bdew.test;

import SteamJni.SteamServerApi;

public class TestSteamAPI {
    private static final String MOD_DIR = "wurmunlimitedserver";
    private static final String PRODUCT_NAME = "wurmunlimitedserver";
    private static final String GAME_DESCRIPTION = "Wurm Unlimited Server";
    private static final String VERSION = "1.0.0.0";
    private static final String GAME_TAGS = "OFFLINE;";
    private static final long SERVER_IP = 0L;
    private static final short STEAM_PORT = 8766;
    private static final short GAME_PORT = 3724;
    private static final short QUERY_PORT = 27016;
    private static final int SERVER_MODE_AUTHENTICATION_AND_SECURE = 3;

    private static final String SERVER_NAME = "Test Server";
    private static final String MAP_NAME = "Test Map";
    private static final int MAX_PLAYERS = 10;

    private static final long CONNECT_TIMEOUT_MS = 15_000;
    private static final long EXTRA_RUN_MS = 3_000;
    private static final long POLL_INTERVAL_MS = 50;

    private static class TestSteamServerApi extends SteamServerApi {
        volatile boolean connected;
        volatile boolean unexpectedAuthResponse;

        @Override
        public void OnSteamServersConnected() {
            SetPasswordProtected(false);
            setMaxPlayerCount(MAX_PLAYERS);
            SetServerName(SERVER_NAME);
            SetBotCount(0);
            SetMapName(MAP_NAME);
            System.out.println("Server connected to steam");
            connected = true;
        }

        @Override
        public void OnValidateAuthTicketResponse(final String steamId, final boolean authenticated) {
            System.out.println("Unexpected auth response for " + steamId + ": " + authenticated);
            unexpectedAuthResponse = true;
        }
    }

    public static void main(final String[] args) throws InterruptedException {
        final TestSteamServerApi api = new TestSteamServerApi();

        api.CreateCallback();

        boolean serverStarted = false;
        boolean failed = false;
        try {
            if (!api.SteamGameServer_Init(SERVER_IP, STEAM_PORT, GAME_PORT, QUERY_PORT, SERVER_MODE_AUTHENTICATION_AND_SECURE, VERSION)) {
                System.out.println("Could not start server");
                api.DeleteCallback();
                System.exit(1);
            }
            api.SetModDir(MOD_DIR);
            api.SetDedicatedServer(true);
            api.SetProduct(PRODUCT_NAME);
            api.SetGameDescription(GAME_DESCRIPTION);
            api.SetGameTags(GAME_TAGS);
            api.LogOnAnonymous();
            api.EnableHeartbeats(true);
            serverStarted = true;
            System.out.println("Starting the server");

            final long deadline = System.currentTimeMillis() + CONNECT_TIMEOUT_MS;
            while (!api.connected && System.currentTimeMillis() < deadline) {
                api.SteamGameServer_RunCallbacks();
                Thread.sleep(POLL_INTERVAL_MS);
            }
            if (!api.connected) {
                System.out.println("Timed out waiting for the Steam connection callback");
                failed = true;
            }

            final long runUntil = System.currentTimeMillis() + EXTRA_RUN_MS;
            while (System.currentTimeMillis() < runUntil) {
                api.SteamGameServer_RunCallbacks();
                Thread.sleep(POLL_INTERVAL_MS);
            }
        } finally {
            if (serverStarted) {
                api.EnableHeartbeats(false);
                api.LogOff();
                api.SteamGameServer_Shutdown();
            }
            api.DeleteCallback();
        }
        failed |= api.unexpectedAuthResponse;
        System.out.println("Shut down");
        if (failed) {
            System.out.println("Test FAILED");
            System.exit(1);
        }
        System.out.println("Test passed");
    }
}