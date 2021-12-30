#ifndef FFMPEGENCODER_H
#define FFMPEGENCODER_H

#include <QObject>

struct ffCodecInfo {
	int id;
};

class ffmpegEncoder : public QObject
{
	Q_OBJECT
public:
	explicit ffmpegEncoder(QObject *parent = 0);

signals:

public slots:

};

#endif // FFMPEGENCODER_H
