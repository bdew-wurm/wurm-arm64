#include "net_bdew_test_Greeter.h"
#include <string>
#include <cstdio>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *, void *) {
    printf("greeter.dll loaded\n");
    fflush(stdout);
    return JNI_VERSION_1_8;
}

JNIEXPORT jstring JNICALL Java_net_bdew_test_Greeter_greet(JNIEnv *env, jobject, jstring name) {
    const char *nameChars = env->GetStringUTFChars(name, nullptr);
    std::string result = std::string("Hello, ") + nameChars + "!";
    env->ReleaseStringUTFChars(name, nameChars);
    return env->NewStringUTF(result.c_str());
}
