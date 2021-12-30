#ifndef FAHRPLAN_H
#define FAHRPLAN_H

#include <QObject>
#include <QImage>
#include <QFont>
#include <QFontMetrics>
#include <QTransform>
#include <QPainter>
#include "irenderer.h"
#include <QDebug>
#include <QJsonObject>
#include <QDate>
#include <QPen>
#include <QRegion>
struct fahrplanEvent
{
	QString title;
	QDateTime start;
	int day;
	int startMinute;
	int duration; ///< duration in seconds
	QString description;
	QString comments;
	QString stage;
};
struct fahrplanTrack {
	QString name;
	QColor boxColor;
	QColor frameColor;
	QList<fahrplanEvent> events;
};
class Fahrplan : public iRenderer
{
	//Q_OBJECT

public:
	Fahrplan(int w, int h, QObject * parent = NULL);
	~Fahrplan();
	bool loadFahrplan(QByteArray d);
	bool loadTrack(QJsonObject fahrplanTrack);
//	void pushElement(iMessageElement  *);
	virtual void renderFrame(QPainter *, quint64 frameID=0);

	int eventDay(fahrplanEvent evt);
private:
	QList<fahrplanTrack> mTracks;
	QStringList mTrackNames;
	QDate mDayZero;
	QString mEventTitle;
	int mTrackWidth;
	int mTrackSpace;
	int mPadding ;
	int mSpeed;
	int mHeaderSize;
	QRegion mEventRegion;
	QFont mTitleFont;
	QFont mSmallFont;
	void drawEvent(QPainter *p, QRect rect,
	               QPen borderPen, QBrush fillBrush,
	               const fahrplanEvent &evt);
signals:

public slots:
};

#endif // MARQUEE_H

