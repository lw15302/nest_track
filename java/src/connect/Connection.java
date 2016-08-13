package connect;

public class Connection {
    private NativeClient client;
    private boolean connectionStatus;
    private boolean trackingStatus;

    public Connection() {
        client = new NativeClient();
    }

    public void open() {
        connectionStatus = client.connect();
    }

    public void track() {
        trackingStatus = client.track();
    }

    public void stopTrack() {
        client.stopTrack();
    }

    public boolean getConnectionStatus() {
        return connectionStatus;
    }

    public boolean getTrackingStatus() {
        return trackingStatus;
    }
}
