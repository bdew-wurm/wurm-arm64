package net.bdew.test;

public class Greeter {
    public native void greet(String name);

    public void onGreet(String text) {
        System.out.println(text);
    }

    static {
        System.loadLibrary("greeter");
    }
}
