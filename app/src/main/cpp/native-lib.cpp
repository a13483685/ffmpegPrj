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
using namespace std;
//判断分母是否为零,防止分母为0 将分数转化为带浮点的小数
static double r2d(AVRational r)
{
    return r.den==0||r.num==0?0:(double)r.num/(double)r.den;
}


extern "C"
JNIEXPORT jstring

JNICALL
Java_demo_test_com_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello +=avcodec_configuration();
    //1.初始化解封装,注册解码器
    av_register_all();
    //2.初始化网络
    avformat_network_init();
    //3.打开文件

    //解码器初始化
    avcodec_register_all();
    AVFormatContext *ic = NULL;
    char path[] = "/sdcard/1080.mp4";
    int ret = avformat_open_input(&ic,path,0,0);
//    int ret = avformat_find_stream_info(ic,0);
    if (ret != 0){
        LOGW("File %s open failed",av_err2str(ret));
//        __android_log_print(ANDROID_LOG_WARN,"avformat_open_input %s success!",path);

    }
    LOGW("File %s open sucess!",path);
    LOGW("duration = %lld nb_streams = %d",ic->duration,ic->nb_streams );
//获取流信息
    ret = avformat_find_stream_info(ic,0);
    if(ret!=0){
        LOGW("avformat_find_stream_info failed !");
    }
    LOGW("duration = %lld nb_streams = %d",ic->duration,ic->nb_streams );

    /**
     * 打印视频的信息
     * nb_streams:里面有的是音频，有的是视频
     *无法通过下标判断哪个是音频，哪个是视频，需要逐一判断
     *
     *
     */

    int videoStream =0;
    int fps = 0;
    int audioStream = 0;
    for (int i=0;i<ic->nb_streams;i++)
    {
        AVStream *as = ic->streams[i];
        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            LOGW("视频数据");
            videoStream = i;
            fps = r2d(as->avg_frame_rate);
            LOGW("fps = %d,width = %d height =%d codeid = %d pixformat=%d",fps,
                as->codecpar->width,
                 as->codecpar->height,
                 as->codecpar->codec_id,
                 as->codecpar->format
            );

        } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            LOGW("音频数据");
            audioStream = i;
            LOGW("sample_rate=%d channels=%d sample_format=%d",
                as->codecpar->sample_rate,
                as->codecpar->channels,
                 as->codecpar->format
            );
        }
    }
    //获取音频流信息
    audioStream = av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    LOGW("av_find_best_stream audioStream = %d",audioStream);

    //解码器也要初始化

    //软解码器
    AVCodec *codec = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id);
//    codec = avcodec_find_decoder_by_name("h264_mediacodec");
    if (!codec)
    {
        LOGW("avcodec_find failed!");
        return env->NewStringUTF(hello.c_str());
    }
    AVCodecContext *cc = avcodec_alloc_context3(codec);
    //
    avcodec_parameters_to_context(cc,ic->streams[videoStream]->codecpar);
    cc->thread_count = 1;
    //开启解码器
    ret = avcodec_open2(cc,0,0);
    if(ret!=0)
    {
        LOGW("AVCODEC_OPEN2 FAILED!");
        return env->NewStringUTF(hello.c_str());
    }


    AVPacket *pkt = av_packet_alloc();
    for (;;)
    {
        int re = av_read_frame(ic,pkt);
        if(re!=0)
        {
            LOGW("读取到结尾处");
            int pos = 20*r2d(ic->streams[videoStream]->time_base);
            av_seek_frame(ic,videoStream,pos,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
            continue;
        }
        LOGW("stream = %d size = %d pts =%lld flag=%d",pkt->stream_index,pkt->size,pkt->pts,pkt->flags);
        av_packet_unref(pkt);
    }


    avformat_close_input(&ic);
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