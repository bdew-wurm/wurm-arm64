package net.bdew.test;

public class Main {
    public static void main(String[] args) {
        System.out.println("Current working directory: " + System.getProperty("user.dir"));
        System.out.println("java.library.path: " + System.getProperty("java.library.path"));

        Greeter test = new Greeter();
        test.greet("bdew");
    }
}