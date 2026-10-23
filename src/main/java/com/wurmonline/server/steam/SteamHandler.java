package com.wurmonline.server.steam;

public abstract class SteamHandler {
    public abstract void onValidateAuthTicketResponse(final String steamIdString, final boolean wasSucces);
    public abstract void onSteamConnected();
}
