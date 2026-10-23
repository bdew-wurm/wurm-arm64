package net.bdew.test;

import com.wurmonline.server.steam.SteamHandler;
import com.wurmonline.server.steam.SteamServerApi;

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

    private static class TestHandler extends SteamHandler {
        volatile SteamServerApi api;
        volatile boolean connected;

        @Override
        public void onSteamConnected() {
            api.SetPasswordProtected(false);
            api.setMaxPlayerCount(MAX_PLAYERS);
            api.SetServerName(SERVER_NAME);
            api.SetBotCount(0);
            api.SetMapName(MAP_NAME);
            System.out.println("Server connected to steam");
            connected = true;
        }

        @Override
        public void onValidateAuthTicketResponse(final String steamIdString, final boolean wasSucces) {
            System.out.println("Unexpected auth response for " + steamIdString + ": " + wasSucces);
        }
    }

    public static void main(final String[] args) throws InterruptedException {
        final TestHandler handler = new TestHandler();
        final SteamServerApi api = new SteamServerApi(handler);
        handler.api = api;

        api.CreateCallback();

        boolean serverStarted = false;
        try {
            if (!api.SteamGameServer_Init(SERVER_IP, STEAM_PORT, GAME_PORT, QUERY_PORT, SERVER_MODE_AUTHENTICATION_AND_SECURE, VERSION)) {
                System.out.println("Could not start server");
                return;
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
            while (!handler.connected && System.currentTimeMillis() < deadline) {
                api.SteamGameServer_RunCallbacks();
                Thread.sleep(POLL_INTERVAL_MS);
            }
            if (!handler.connected) {
                System.out.println("Timed out waiting for the Steam connection callback");
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
        System.out.println("Shut down");
    }
}