#include "net_bdew_test_Greeter.h"
#include <string>
#include <cstdio>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *, void *)
{
    printf("greeter.dll loaded\n");
    fflush(stdout);
    return JNI_VERSION_1_8;
}

JNIEXPORT void JNICALL Java_net_bdew_test_Greeter_greet(JNIEnv *env, jobject self, jstring name)
{
    const char *nameChars = env->GetStringUTFChars(name, nullptr);
    std::string result = std::string("Hello, ") + nameChars + "!";
    env->ReleaseStringUTFChars(name, nameChars);

    jclass cls = env->GetObjectClass(self);
    jmethodID onGreet = env->GetMethodID(cls, "onGreet", "(Ljava/lang/String;)V");
    if (onGreet == nullptr)
    {
        return;
    }
    jstring text = env->NewStringUTF(result.c_str());
    env->CallVoidMethod(self, onGreet, text);
    env->DeleteLocalRef(text);
    env->DeleteLocalRef(cls);
}
