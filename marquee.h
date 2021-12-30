#ifndef MARQUEE_H
#define MARQUEE_H

#include <QObject>
#include <QImage>
#include <QFont>
#include <QFontMetrics>
#include <QTransform>
#include <QPainter>
#include "irenderer.h"
#include <QDebug>
class iMessageElement
{
public:
	explicit iMessageElement(int height = -1) : mHeight(height), mReQueue(false), mRefCount(1)
	{

	}
	bool reQueue() const
	{
		return true;
	}

	void setReQueue( bool v)
	{
		mReQueue = v;
	}

	int width()
	{
		return mImg.width();
	}

	virtual void paint(QTransform &) = 0;

	QImage getImage()
	{
		return mImg;
	}

	int addRef()
	{
		int		oldValue;

		oldValue = mRefCount.fetchAndAddAcquire(1);
		qDebug()<<oldValue +1;
		return (oldValue + 1);
	}
	int release ()
		{
		int		oldValue;
		oldValue = mRefCount.fetchAndAddAcquire(-1);
		if (oldValue == 1)
		{
			delete this;
		}
		qDebug()<<"release"<<oldValue -1;
		return (oldValue - 1);
	}

protected:

	int mHeight;
	bool mReQueue;
	QImage mImg;

private:
	QAtomicInt mRefCount;
};

class textElement : public iMessageElement
{
public:
	explicit textElement(QString text,
						 QFont fnt,
						 int height,
						 QColor color = Qt::white)
		: iMessageElement(height), mFont(fnt), mColor(color)
	{
		mText = text;
	}
	virtual void paint(QTransform & xfm)
	{
		QFontMetrics fm(mFont);
		QRect rct = fm.boundingRect(mText);
		int w = xfm.mapRect(rct).width();
		mImg = QImage(w, mHeight, QImage::Format_ARGB32_Premultiplied);
		QRect other = xfm.inverted().mapRect(mImg.rect());
		mImg.fill(Qt::transparent);
		QRect target = fm.boundingRect(other, Qt::AlignCenter, mText);
		QPainter p(&mImg);
		p.setTransform(xfm);
		p.setFont(mFont);
		p.setPen(mColor);

		p.drawText(target, mText);
	}

private:
	QFont mFont;
	QColor mColor;
	QString mText;
};


class Marquee : public iRenderer
{
	//Q_OBJECT

public:
	Marquee(int w, int h, QObject * parent = NULL);
	~Marquee();
	struct activeEntry
	{
		activeEntry(iMessageElement * e, int x_)
		{
			element = e;
			x = x_;
		}

		iMessageElement * element;
		int x;
	};
	void pushElement(iMessageElement  *);
	virtual void renderFrame(QPainter *, quint64 frameID=0);

private:
	QList<iMessageElement * > mQueue;
	QList<activeEntry> mDisplayList;
	void popNextElement();
	int mSpacing;
	int mSpeed;
signals:

public slots:
};

#endif // MARQUEE_H
