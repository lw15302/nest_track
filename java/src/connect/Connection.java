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

    public int[] getData(String[] ip) {
            int[] data = client.getData(ip);
            try {
                Thread.sleep(1);
            } catch (Exception e) {
                e.printStackTrace();
            }
        return data;
    }

    public boolean getConnectionStatus() {
        return connectionStatus;
    }

    public boolean getTrackingStatus() {
        return trackingStatus;
    }

    public void setTrackingStatus(boolean trackingStatus) {
        this.trackingStatus = trackingStatus;
    }
}
