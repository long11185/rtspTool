#include "rtspthread.h"
#include <QDebug>

RtspThread::RtspThread(QObject *parent) :
    QThread(parent)
{

}

void  RtspThread::run()
{    
    //qDebug()<<"this is ";
    ffmpeg->Play();
}
