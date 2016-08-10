package connect;

public class NativeClient extends Thread {
    public native void connect();
    public native void disconnect();

    static {
        System.loadLibrary("server");
        System.out.println("inside static block");
    }

    @Override
    public void run() {
        connect();
    }
}

