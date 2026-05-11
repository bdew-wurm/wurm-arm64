package net.bdew.test;

public class Greeter {
    private native String greet(String name);

    public String doGreet(String name) {
        return greet(name);
    }

    static {
        System.loadLibrary("greeter");
    }
}
