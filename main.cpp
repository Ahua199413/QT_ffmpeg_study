#include "mainwindow.h"

#include <stdio.h>
#include <QApplication>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavdevice/avdevice.h"
    #include "libavfilter/avfilter.h"
    #include "libavformat/avformat.h"
}
int main(int argc, char *argv[])
{
    //printf("avcodec version is %u \n",avcodec_version());
    //printf("avdevice version is %u \n",avdevice_version());
    //printf("avfilter version is %u \n",avfilter_version());
    //printf("avfilter version is %u \n",avformat_version());
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Player"));
    w.show();
    return a.exec();
}
