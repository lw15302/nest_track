package connect;

public class NativeClient extends Thread {
    public boolean status;

    public native boolean connect();

    static {
        System.loadLibrary("server");
    }

    @Override
    public void run() {
        status = connect();
    }

    public boolean getStatus() {
        return status;
    }
}

