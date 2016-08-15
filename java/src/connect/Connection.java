package connect;

public class Connection {
    private NativeClient client;
    private boolean connectionStatus;
    private boolean trackingStatus;

    public Connection() {
        client = new NativeClient();
    }

    public void open(String[] ip) {
        connectionStatus = client.connect(ip);
    }

    public void track(String[] ip) {
        trackingStatus = client.track(ip);
    }

    public void stopTrack(String[] ip) {
        client.stopTrack(ip);
    }

    public boolean getConnectionStatus() {
        return connectionStatus;
    }

    public boolean getTrackingStatus() {
        return trackingStatus;
    }
}
