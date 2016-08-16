package connect;

public class NativeClient {
    public native boolean connect(String[] ip);
    public native boolean track(String[] ip);
    public native boolean stopTrack(String[] ip);
    public native int[] getData(String[] ip);

    static {
        System.loadLibrary("server");
    }
}



