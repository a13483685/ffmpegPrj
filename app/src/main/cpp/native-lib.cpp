#include <jni.h>
#include <string>
extern "C" {
#include <libavcodec/avcodec.h>

}

extern "C"
JNIEXPORT jstring

JNICALL
Java_demo_test_com_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello +=avcodec_configuration();
    return env->NewStringUTF(hello.c_str());
}
