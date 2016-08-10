package connect;

public class NativeClient extends Thread {
    private boolean trackingStatus;

    public native boolean connect();
    public native boolean track();

    static {
        System.loadLibrary("server");
    }

    @Override
    public void run() {
        trackingStatus = track();
    }


    public boolean getTrackingStatus() {
        return trackingStatus;
    }

}

