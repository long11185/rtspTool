#include "qffmpeg.h"
#include <QDateTime>
#include <QDebug>


QFFmpeg::QFFmpeg(QObject *parent) :
    QObject(parent)
{    
    videoStreamIndex=-1;
}

QFFmpeg::~QFFmpeg()
{
    sws_freeContext(pSwsContext);
}

bool QFFmpeg::Init()
{
    if (url.isEmpty())
        return false;

    avcodec_register_all();//注册所有解码器
    av_register_all();//注册所有格式
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行

    pAVFormatContext = avformat_alloc_context();
    pAVFrame=av_frame_alloc();

    //打开视频流
    int result=avformat_open_input(&pAVFormatContext, url.toStdString().c_str(),NULL,NULL);
    if (result<0){
        qDebug()<<"打开视频流失败";
        return false;
    }

    //获取视频流信息
    result=avformat_find_stream_info(pAVFormatContext,NULL);
    if (result<0){
        qDebug()<<"获取视频流信息失败";
        return false;
    }

    //获取视频流索引
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;qDebug()<<i;
            break;
        }
    }

    if (videoStreamIndex==-1){
        qDebug()<<"获取视频流索引失败";
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    if (pAVCodecContext == NULL){
        qDebug()<<"不能对该视频流进行解码";
        return false;
    }
    videoWidth=pAVCodecContext->width;
    videoHeight=pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);

    pSwsContext = sws_getContext(videoWidth,videoHeight,AV_PIX_FMT_YUV420P,videoWidth,videoHeight,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
        qDebug()<<"打开解码器失败";
        return false;
    }

    qDebug()<<"初始化视频流成功";
    return true;
}

void QFFmpeg::Play()
{
     qDebug()<<"进入线程";
    //一帧一帧读取视频
    int frameFinished=0;
    while (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
        if(pAVPacket.stream_index==videoStreamIndex){
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
            if (frameFinished){
                qDebug()<<"开始解码"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                mutex.lock();
                int result = sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                //发送获取一帧图像信号
                QImage image(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);
                emit GetImage(image);
                mutex.unlock();
                if (result<0){
                    qDebug()<<"解码出错"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                }
            }
        }
        //释放资源,否则内存会一直上升
        av_free_packet(&pAVPacket);
    }
}
