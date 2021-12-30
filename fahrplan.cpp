#include "fahrplan.h"
#include<QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>
#include <QTextOption>
Fahrplan::Fahrplan(int w, int h, QObject *parent) : iRenderer(w, h)//, QObject(parent)
{
	int bandHeight = 0;
	int fontHeight=32;
	int x0, y0;
	mParameters.insert("y", h*85/100);
	mParameters.insert("x0", x0=w/10);
	mParameters.insert("y0", y0=h/20);
	mParameters.insert("height", bandHeight = h * 8 / 100);
	mParameters.insert("fontSize", fontHeight = h/35);
	mParameters.insert("font", "verdana");
	mParameters.insert("trackSpacing", fontHeight / 2);
	mParameters.insert("hoursOnScreen", 12);
	mPadding = 0;
	mTrackSpace = 8;
	mHeaderSize = fontHeight*1.5;
	QFont f = QFont(mParameters["font"].toString());
	f.setPixelSize(fontHeight);
	f.setBold(false);
	mTitleFont = f;
	f.setFamily("Source Sans Pro");
	f.setPixelSize(fontHeight*2/3);
	f.setBold(false);
	mSmallFont = f;
	mEventRegion = QRegion(QRect(x0, y0+mHeaderSize,mWidth,mHeight-y0*2-mHeaderSize));
//	for(int u=0;u<mWidth;u+=mHeaderSize)
//	{
//		QRect area(x0 + u, y0 + mHeaderSize/2, mHeaderSize, mHeaderSize);
//		mEventRegion-=QRegion(area,QRegion::Ellipse);
//	}
}

Fahrplan::~Fahrplan()
{

}

bool Fahrplan::loadFahrplan(QByteArray d)
{
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(d, &err);
	if (err.error != QJsonParseError::NoError)
		return false;
	if (!doc.isObject())
		return false;
	QJsonObject rootObj = doc.object();
//	qDebug()<<rootObj.value("day0").toString();
	mDayZero = QDate::fromString(rootObj.value("day0").toString(),"yyyy-MM-dd");
	mEventTitle = rootObj.value("event").toString();
//	qDebug() << mEventTitle << mDayZero;

	foreach(QJsonValue track, rootObj.value("tracks").toArray())
	{
		if (!track.isObject())
			return false;
		if (!loadTrack(track.toObject()))
			return false;
	}
	return true;
}

bool Fahrplan::loadTrack(QJsonObject track)
{
	QString trackTitle = track.value("track").toString();
	qDebug()<<"Loading track"<<trackTitle;
	QList<fahrplanEvent> trackData;
	foreach(QJsonValue v, track.value("events").toArray())
	{
		if (!v.isObject())
			continue;
		QJsonObject event = v.toObject();

		int day = event.value("day").toInt();
		int start = event.value("start").toInt();
//		QTime startTime = QTime(start / 60, start % 60);

		QDateTime startDateTime = QDateTime(mDayZero.addDays(day-1), QTime(0,0));
		startDateTime.addSecs(start*60);
		QString title = event.value("title").toString();
		QString stage = event.value("stage").toString();
		QString description = event.value("description").toString();
		QString notes = event.value("comments").toString();
		int duration = event.value("duration").toInt();
		fahrplanEvent evt;
		evt.start = startDateTime;
		evt.title = title;
		evt.duration = duration;
		evt.description = description;
		evt.startMinute = start;
		evt.comments = notes;
		evt.stage = stage;
		evt.day = day;
		trackData.push_back(evt);
	}
	std::sort(trackData.begin(), trackData.end(),
	          [](const fahrplanEvent & a, const fahrplanEvent & b)
	{ return (a.start < b.start);}
	          );
	foreach(auto evt, trackData)
	{
		qDebug()<<evt.title
		       <<evt.start.time().toString("HH:mm")
		      <<"to"
		     <<evt.start.time().addSecs(60*evt.duration).toString("HH:mm");
	}
	fahrplanTrack trackObj;
	trackObj.events = trackData;
	trackObj.name = trackTitle;
	trackObj.frameColor = Qt::white;
	trackObj.boxColor = Qt::lightGray;
	if (track.contains("frameColor"))
		trackObj.frameColor = QColor(track.value("frameColor").toString());
	if (track.contains("boxColor"))
		trackObj.boxColor = QColor(track.value("boxColor").toString());
	mTracks.push_back(trackObj);
	return true;

}

//void Fahrplan::pushElement(iMessageElement * e)
//{
//	mQueue.push_back(e);
//}

/*
 * int th = 32;
	QFont f = QFont("sans");
	f.setPixelSize(32);
	textElement * te = new textElement("Test", f, th);
	textElement * splitter = new textElement("+++", f, th);
	te->paint(m_Transformation);
	splitter->paint(m_Transformation);
	mQueue.push_back(te);
	mQueue.push_back(splitter);
	mQueue.push_back(te);
	mQueue.push_back(splitter);
	mQueue.push_back(te);
	mQueue.push_back(splitter);
	mQueue.push_back(te);
	mQueue.push_back(splitter);
	mQueue.push_back(te);
	mQueue.push_back(splitter);
	pushNextElement();
	*/

void Fahrplan::drawEvent(QPainter * p, QRect rect, QPen borderPen, QBrush fillBrush, const fahrplanEvent & evt)
{
	int y0      = mParameters.value("y0", 0).toInt();
	QColor fontColor = (fillBrush.color().lightness() > 150)?Qt::black:Qt::white;
	p->setPen(borderPen);
	p->setRenderHint(QPainter::Antialiasing, true);
	p->setRenderHint(QPainter::TextAntialiasing, true);
	p->setBrush(fillBrush);
	p->drawRect(rect);
	p->setPen(fontColor);
	p->setBrush(Qt::NoBrush);
	QFontMetrics fm(mTitleFont);
	rect.adjust(mPadding, mPadding, -mPadding,-mPadding);
	QRect txRect = rect;
	int flags = Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap;
	txRect = fm.boundingRect(txRect, flags, evt.title);
	if (rect.height() < txRect.height() * 2)
	{
		flags&=~Qt::AlignTop;
		flags|=Qt::AlignVCenter;
		txRect = rect;
		txRect = fm.boundingRect(txRect, flags, evt.title);
	}

	if (txRect.top() < y0)
		txRect.moveTop(y0 + mHeaderSize);
	if (txRect.bottom() > rect.bottom())
		txRect.moveBottom(rect.bottom());
//	textOption.setWrapMode(QTextOption::WordWrap);
//	textOption.setAlignment(Qt::AlignHCenter| Qt::AlignTop);
	p->setFont(mTitleFont);
	p->drawText(txRect, flags, evt.title);
}

int Fahrplan::eventDay(fahrplanEvent evt)
{
	mDayZero.daysTo(evt.start.date()) + 1;
}
void Fahrplan::renderFrame(QPainter * p, quint64 frameID)
{

	int y       = mParameters.value("y", mHeight*8/10).toInt();
	int x0      = mParameters.value("x0", 0).toInt();
	int y0      = mParameters.value("y0", 0).toInt();
	int height  = mParameters.value("height", mHeight/10).toInt();
	mPadding     = mParameters.value("spacing", mWidth / 100).toInt();
	mSpeed      = mParameters.value("speed", 3).toInt();

	QTransform otx = p->transform();
	QTransform unity;
	unity.reset();
	p->setTransform(unity);
	int day = mDayZero.daysTo(QDate::currentDate());
	if (day<0)
		day=0;
	++day;
//	qDebug()<<"processing for day"<<day;
	int dayEnd = 1440; // 0:00 next day
	int firstTalk = dayEnd;
	foreach(auto track, mTracks)
	{
		foreach(auto evt, track.events)
		{
			if (evt.day == day)
			{
				int eventEnd = evt.startMinute + evt.duration;
				if (eventEnd>dayEnd)
					dayEnd = eventEnd;
				if (evt.startMinute<firstTalk)
					firstTalk = evt.startMinute;
			}
		}
	}
//	qDebug()<<"Day"<<day<<"has"<<dayEnd/60<<"hours";
	int nTracks = mTracks.size();

//	fahrplanEvent evt = mTracks.first().first();
	int trackNr=0;
	int pxPerMinute = 3;
	int minutesOnScreen = (mHeight - 2 * y0) / pxPerMinute;

	//draw legend
	QFontMetrics fm(mSmallFont);
	int legendWidth = fm.boundingRect("00:00").width() + mTrackSpace;
	int widthForTracks = mWidth - x0*2 - legendWidth;

	int trackWidth = qMin((widthForTracks - nTracks * mTrackSpace)  / 2, (widthForTracks - nTracks * mTrackSpace) / qMax(1, nTracks));
//	if (trackWidth < 100)
//		trackWidth = 100;
	//make this relative to the start of the current congress day
	int m0 = QTime::currentTime().hour() * 60
	         + QTime::currentTime().minute();
	int currentMinute = m0;
	m0-=minutesOnScreen / 2;
	if (m0<0)
		m0=0;
	if (m0 < (firstTalk - 60))
		m0=firstTalk - 60;
	if (minutesOnScreen > (dayEnd - m0))
		m0 = dayEnd - minutesOnScreen;

	fm = QFontMetrics(mTitleFont);
	p->setFont(mTitleFont);
	foreach(auto track, mTracks)
	{
		QString title = track.name;
		QRect textRect(x0 + legendWidth + trackNr * (trackWidth + mTrackSpace),
		               y0, trackWidth,
		               mTitleFont.pixelSize());
		int flags = Qt::AlignCenter;
		textRect = fm.boundingRect(textRect, flags, title);
		p->setPen(Qt::white);
		p->drawText(textRect, title);
		++trackNr;
	}

	QPen linePen(Qt::white);
	QPen textPen(Qt::white);
	linePen.setStyle(Qt::DotLine);
	p->setFont(mSmallFont);
	for (int m=0;m<dayEnd; m+=30)
	{
		QString txt = QString::asprintf("%02d:%02d", (m/60)%24, m%60);
		int lineY = y0 + pxPerMinute * (m-m0);
		int pxs = mSmallFont.pixelSize() * 2;
		QRect destRect(x0, lineY-pxs, legendWidth, pxs*2);
		QRect fontRect = fm.boundingRect(destRect, Qt::AlignVCenter | Qt::AlignLeft, txt);
		if (lineY<(y0+mHeaderSize))
			continue;
		if (lineY>(mHeight - y0))
			continue;
		p->setPen(textPen);
		p->drawText(fontRect, txt);
		p->setPen(linePen);
		p->drawLine(x0, lineY, mWidth-x0,lineY);
	}

	trackNr = 0;
	p->save();
	p->setClipRegion(mEventRegion);
	foreach(auto track, mTracks)
	{
		QBrush boxBrush(track.boxColor);//qRgba(0,0,64, 200));
		QPen borderPen(track.frameColor);//qRgb(32, 192,255));
		borderPen.setWidth(2);
		foreach(auto evt, track.events)
		{
			if (evt.day!=day)
				continue;
			int y1 = y0 + (evt.startMinute-m0) * pxPerMinute;
			int h = evt.duration * pxPerMinute;
			int y2=y1+h;
			if ((y1<y0) && (y2<y0))
				continue;
			QRect eventRect(x0 + legendWidth + trackNr * (trackWidth + mTrackSpace), y1
			                , trackWidth, h);
			drawEvent(p, eventRect,borderPen , boxBrush,
			           evt);
		}
		trackNr++;
	}
	p->restore();
//	QDateTime dayStart = QDateTime(QDate::currentDate(), QTime(0,0));
//	int m = -QDateTime::currentDateTime().secsTo(dayStart);
	int lineY = y0 + pxPerMinute * (currentMinute-m0);
	p->setPen(Qt::red);
	p->drawLine(x0, lineY, mWidth-x0,lineY);

}
