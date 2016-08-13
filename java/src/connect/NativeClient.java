package connect;

public class NativeClient {
    public native boolean connect();
    public native boolean track();
    public native boolean stopTrack();

    static {
        System.loadLibrary("server");
    }
}


