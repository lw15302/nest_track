package connect;

public class Connection {
    private NativeClient client;
    private boolean connectionStatus;

    public Connection() {
        client = new NativeClient();
    }

    public void open() {
        connectionStatus = client.connect();
    }

    public void track() {
        client.start();
    }

    public boolean getConnectionStatus() {
        return connectionStatus;
    }

    public boolean getTrackingStatus() {
        return client.getTrackingStatus();
    }
}
