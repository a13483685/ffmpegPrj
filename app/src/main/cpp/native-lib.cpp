#include <jni.h>
#include <string>
#include <android/log.h>
//#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN,"testff",___VA_ARGS___)
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
extern "C"
JNIEXPORT jboolean JNICALL
Java_demo_test_com_myapplication_MainActivity_open(JNIEnv *env, jobject instance, jstring url_,
                                                   jobject handle) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // TODO
    FILE *fp = fopen(url,"rb");
    if(!fp) {
//        LOGW("%s open failed !",url_);
        __android_log_print(ANDROID_LOG_WARN,url,"open failed");
    }
    else
    {
        __android_log_print(ANDROID_LOG_WARN,url,"open success!");
        fclose(fp);
    }

    env->ReleaseStringUTFChars(url_, url);
}