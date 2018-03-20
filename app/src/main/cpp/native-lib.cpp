#include <jni.h>
#include <string>
#include <android/log.h>
//___VA_ARGS___
//#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN,"testff",)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"testff" ,__VA_ARGS__) // 定义LOGW类型

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

extern "C"
JNIEXPORT jstring

JNICALL
Java_demo_test_com_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello +=avcodec_configuration();
    //1.初始化解封装
    av_register_all();
    //2.初始化网络
    avformat_network_init();
    //3.打开文件
    AVFormatContext *ic = NULL;
    char path[] = "/sdcard/1080.mp4";
    int ret = avformat_open_input(&ic,path,0,0);
    if (ret == 0){
//        __android_log_print(ANDROID_LOG_WARN,"avformat_open_input %s success!",path);
        LOGW("File %s open sucess!",path);
    } else {
//        __android_log_print(ANDROID_LOG_WARN,"avformat_open_input error %s!",av_err2str(ret));
        LOGW("File %s open failed",av_err2str(ret));
    }
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
    return true;
}