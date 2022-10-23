#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    avcodec_configuration();
    unsigned version = avcodec_version();
    QString ch = QString::number(version, 10);
    qDebug() << "version: " << version;
    qDebug() << "currentPath: " << QDir::currentPath();

    // Init
    AVFormatContext *fmt_ctx = avformat_alloc_context();
    AVFormatContext *fmtCtx=NULL;
    AVPacket *pkt =NULL;
    AVCodecContext *codecCtx=NULL;
    AVCodecParameters *avCodecPara=NULL;
    const AVCodec *codec=NULL;
    int ret=0;
    //char* fileName="/Sample.flv";//文件地址

    QString str1 = QDir::currentPath() + "/Sample.flv";

    qDebug() << "currentPath: " << str1;
    QByteArray ba = str1.toLocal8Bit();
    const char *fileName = ba.data();


    do{
        //  open
        if ((ret = avformat_open_input(&fmt_ctx, fileName, NULL, NULL))<0) {
            printf("ret = %d\n", ret);
            break;//Cannot open video file
        }
        // find stream
        if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
            printf("Cannot find stream information\n");
            break;
        }
        //  dump format
        av_dump_format(fmt_ctx,0,fileName,0);
    }while(0);
    // close
    avformat_close_input(&fmt_ctx);
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnPlay_clicked()
{
    ui->wgtPlayer->setUrl(ui->lineUrl->text());
    ui->wgtPlayer->play();
}


void MainWindow::on_btnStop_clicked()
{
    ui->wgtPlayer->stop();
}

