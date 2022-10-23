#include "ffmpegwidget.h"

FFmpegVideo::FFmpegVideo()
{
    fmt_ctx     = avformat_alloc_context(); // 分配一個AVFormatContext，FFMPEG所有的操作都要通過這個AVFormatContext來進行
    pkt         = av_packet_alloc();        // 分配一個AAVPacket
    yuvFrame    = av_frame_alloc();
    rgbFrame    = av_frame_alloc();
}

FFmpegVideo::~FFmpegVideo()
{
    if(!pkt) av_packet_free(&pkt);
    if(!yuvFrame) av_frame_free(&yuvFrame);
    if(!rgbFrame) av_frame_free(&rgbFrame);
    if(!videoCodecCtx) avcodec_free_context(&videoCodecCtx);
    if(!videoCodecCtx) avcodec_close(videoCodecCtx);
    if(!fmt_ctx) avformat_close_input(&fmt_ctx);
}

void FFmpegVideo::setUrl(QString url)
{
    _url = url;
}

int FFmpegVideo::open_input_file()
{
    if(_url.isEmpty()) return -1;

    // open
    if(avformat_open_input(&fmt_ctx, _url.toLocal8Bit().data(), NULL,NULL) < 0){
        qDebug() << "Cannot open input file.\n";
        return -1;
    }

    // find
    if(avformat_find_stream_info(fmt_ctx, NULL) < 0){
        qDebug() << "Cannot find any stream in file.\n";
        return -1;
    }
    // dump information
    av_dump_format(fmt_ctx, 0, _url.toLocal8Bit().data(), 0);

    // 循環查找AVMEDIA_TYPE_VIDEO類型的stream
    // 便將其記錄下來 保存到videoStreamIndex
    int streamCnt = fmt_ctx->nb_streams;
    for(int i = 0; i < streamCnt; i++){
        if(fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
            continue;
        }
    }

    // 如果videoStream为-1, 没有找到videoStream
    if(videoStreamIndex == -1){
        qDebug() << "Cannot find video stream in file.\n";
        return -1;
    }

    // find Decoder
    AVCodecParameters *videoCodecPara = fmt_ctx->streams[videoStreamIndex]->codecpar;

    if(!(videoCodec = avcodec_find_decoder(videoCodecPara->codec_id))){
        qDebug() << "Cannot find valid decode codec.\n";
        return -1;
    }

    // according to videoCodecPara to create 解碼器內容
    if(!(videoCodecCtx = avcodec_alloc_context3(videoCodec))){
        qDebug() << "Cannot find valid decode codec context.\n";
        return -1;
    }
    if(avcodec_parameters_to_context(videoCodecCtx, videoCodecPara) < 0){
        qDebug() << "Cannot initialize parameters.\n";
        return -1;
    }

    // open Decoder
    if(avcodec_open2(videoCodecCtx, videoCodec, NULL) < 0){
        qDebug() << "Cannot open codec.\n";
        return -1;
    }

    img_ctx = sws_getContext(videoCodecCtx->width,          // SrcW
                             videoCodecCtx->height,         // SrcH
                             videoCodecCtx->pix_fmt,        // Src Format
                             videoCodecCtx->width,          // DstW
                             videoCodecCtx->height,         // DstH
                             AV_PIX_FMT_RGB32,              // Dst Format
                             SWS_BICUBIC,NULL,NULL,NULL);   // algorithm and options to use for rescaling
    // one image frame size
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32,videoCodecCtx->width,videoCodecCtx->height,1);
    out_buffer = (unsigned char *)av_malloc(numBytes*sizeof(unsigned char));

    //會將pFrameRGB的數據按RGB格式自動"關聯"到buffer，即pFrameRGB中的數據改變了，out_buffer中的數據也會相應的改變
    int res = av_image_fill_arrays(
                rgbFrame->data, rgbFrame->linesize,
                out_buffer, AV_PIX_FMT_RGB32,
                videoCodecCtx->width, videoCodecCtx->height, 1);
    if(res<0){
        qDebug()<<"Fill arrays failed.";
        return -1;
    }

    return true;
}

void FFmpegVideo::run()
{
    if(!open_input_file()){
        qDebug()<<"Please open file first.";
        return;
    }

    // Read one frame, save to AVPacket pkt
    while(av_read_frame(fmt_ctx, pkt) >= 0){
        if(pkt->stream_index == videoStreamIndex){
            if(avcodec_send_packet(videoCodecCtx, pkt) >= 0){
                int ret;
                while((ret = avcodec_receive_frame(videoCodecCtx, yuvFrame)) >= 0){
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                        return;
                    else if (ret < 0) {
                        fprintf(stderr, "Error during decoding\n");
                        exit(1);
                    }
                    sws_scale(img_ctx,
                              yuvFrame->data, yuvFrame->linesize,
                              0, videoCodecCtx->height,
                              rgbFrame->data, rgbFrame->linesize);

                    QImage img(out_buffer,
                               videoCodecCtx->width, videoCodecCtx->height,
                               QImage::Format_RGB32);
                    emit sendQImage(img);
                    QThread::msleep(30);
                }
            }
            av_packet_unref(pkt);
        }
    }

    qDebug()<<"All video play done";
}

FFmpegWidget::FFmpegWidget(QWidget *parent) : QWidget(parent)
{
    ffmpeg = new FFmpegVideo;
    connect(ffmpeg,SIGNAL(sendQImage(QImage)),this,SLOT(receiveQImage(QImage)));
    connect(ffmpeg,&FFmpegVideo::finished,ffmpeg,&FFmpegVideo::deleteLater);
}

FFmpegWidget::~FFmpegWidget()
{
    if(ffmpeg->isRunning()){
        stop();
    }
}

void FFmpegWidget::setUrl(QString url)
{
    ffmpeg->setUrl(url);
}

void FFmpegWidget::play()
{
    stop();
    ffmpeg->start();
}

void FFmpegWidget::stop()
{
    if(ffmpeg->isRunning()){
        ffmpeg->requestInterruption();
        ffmpeg->quit();
        ffmpeg->wait(100);
    }
    img.fill(Qt::black);
}

void FFmpegWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height());
    painter.drawImage(0,0,img);
}

void FFmpegWidget::receiveQImage(const QImage &rImg)
{
    img = rImg.scaled(this->size());
    update();
}
