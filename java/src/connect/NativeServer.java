package connect;

public class NativeServer  {
    public native void hello();

    static {
        System.loadLibrary("server");
        System.out.println("inside static block");
    }


    public static void main(String[] args) {
        NativeServer m = new NativeServer();
        m.hello();
    }
}

