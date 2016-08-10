package connect;

public class Connection {
    private NativeClient client;

    public Connection() {
        client = new NativeClient();
    }

    public void open() {
        client.start();
    }

    public void close() {

    }

    public boolean getStatus() {
        return client.getStatus();
    }
}
