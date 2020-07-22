

#include "frmmain.h"
#include "ui_frmmain.h"
#include "qffmpeg.h"
#include "rtspthread.h"

#define View_Width 480
#define View_Height 320
frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);    

    tempWidth=View_Width;
    tempHeight=View_Height;
    video1Max=false;
    video2Max=false;
    video3Max=false;
    all=false;
    ui->labVideo1->installEventFilter(this);
    ui->labVideo2->installEventFilter(this);
    ui->labVideo3->installEventFilter(this);
}

frmMain::~frmMain()
{
    delete ui;
}

//处理用户双击对应通道最大化处理
bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    //如果用户按下了回车键,则触发退出信号.
    if (event->type()==QEvent::MouseButtonDblClick) {
        if (obj==ui->labVideo1){
            if (video1Max){
                ui->labVideo2->setVisible(true);
                ui->labVideo3->setVisible(true);
                ui->labImage->setVisible(true);
                tempWidth=480;
                tempHeight=320;
            }else{
                ui->labVideo2->setVisible(false);
                ui->labVideo3->setVisible(false);
                ui->labImage->setVisible(false);
                tempWidth=965;
                tempHeight=645;
            }
            video1Max=!video1Max;
            return true;
        }else if (obj==ui->labVideo2){
            if (video2Max){
                ui->labVideo1->setVisible(true);
                ui->labVideo3->setVisible(true);
                ui->labImage->setVisible(true);
                tempWidth=480;
                tempHeight=320;
            }else{
                ui->labVideo1->setVisible(false);
                ui->labVideo3->setVisible(false);
                ui->labImage->setVisible(false);
                tempWidth=965;
                tempHeight=645;
            }
            video2Max=!video2Max;
            return true;
        }else if (obj==ui->labVideo3){
            if (video3Max){
                ui->labVideo1->setVisible(true);
                ui->labVideo2->setVisible(true);
                ui->labImage->setVisible(true);
                tempWidth=480;
                tempHeight=320;
            }else{
                ui->labVideo1->setVisible(false);
                ui->labVideo2->setVisible(false);
                ui->labImage->setVisible(false);
                tempWidth=965;
                tempHeight=645;
            }
            video3Max=!video3Max;
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void frmMain::on_btnOpen_clicked()
{    
    QFFmpeg *ffmpeg=new QFFmpeg(this);
    connect(ffmpeg,SIGNAL(GetImage(QImage)),this,SLOT(SetImage(QImage)));
    ffmpeg->SetUrl(ui->txtUrl->text());
    ffmpeg->Init();
    RtspThread *rtsp=new RtspThread(this);
    rtsp->setffmpeg(ffmpeg);
    rtsp->start();
}

void frmMain::on_btnGetImage_clicked()
{
    ui->labImage->clear();
    int index=ui->cboxVideo->currentIndex();
    if (index==0){
        if (ui->labVideo1->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo1->pixmap());
    }else if (index==1){
        if (ui->labVideo2->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo2->pixmap());
    }else if (index==2){
        if (ui->labVideo3->pixmap()!=0x0)
            ui->labImage->setPixmap(*ui->labVideo3->pixmap());
    }
}

void frmMain::SetImage(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth,tempHeight));
        ui->labVideo1->setPixmap(pix);
        if (all){//启用三通道同步
            ui->labVideo2->setPixmap(pix);
            ui->labVideo3->setPixmap(pix);
        }
    }
}

void frmMain::on_ckAll_stateChanged(int arg1)
{
    all=arg1!=0?true:false;
}
