#include "wndmain.h"
#include <QApplication>

#include <stdio.h>
#include <QDebug>
extern "C" {
#include <libavcodec/avcodec.h>

}

int main(int argc, char *argv[])
{

//	avcodec_register_all();
//	QString avc_version("%1.%2.%3");
//	quint32 avv = avcodec_version();

//	AVCodec *codec = NULL;
//	AVCodecContext* c = NULL;
//	codec = avcodec_find_encoder(CODEC_ID_MPEG1VIDEO);
////	if (!codec) {
////		fprintf(stderr, "codec not found\n");
////		exit(1);
////	}

//	avc_version = avc_version.arg(avv>>16).arg((avv>>8)&0xff).arg(avv&0xff);
//	qDebug()<<"Using libavcodec"<<avc_version;


//	codec = av_codec_next(NULL);
//	while(codec != NULL)
//	{
//		if (codec->type == AVMEDIA_TYPE_VIDEO)
//		 qDebug()<<codec->long_name;
//		 codec = av_codec_next(codec);
//	}
//	codec = avcodec_find_encoder(AV_CODEC_ID_DVVIDEO);
//	if (!codec) {
//		qDebug()<<"No DV video available";
//	}
//	avcodec_register(codec);
//	c = avcodec_alloc_context3(codec);
//	int profileID=0;
//	if (codec->profiles) {
//		while(codec->profiles[profileID].profile!=FF_PROFILE_UNKNOWN) {
//			AVProfile p = codec->profiles[profileID];
//			qDebug()<<"Profile"<<profileID<<p.name;
//		}
//	} else {
//		qDebug()<<"codec profile enum empty, trying hardcoded default";
//		c->pix_fmt = AV_PIX_FMT_YUV422P;
//		c->width = 720;
//		c->height = 576;
//		c->time_base = av_make_q(1,25);
//	}
//	codec->init(c);

	QApplication a(argc, argv);
	wndMain w;
	w.show();
	return a.exec();
}
