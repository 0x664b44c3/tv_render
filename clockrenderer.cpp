#include "clockrenderer.h"
#include <QPainter>
#include <math.h>
#include <QDateTime>
#include <QTime>

ClockRenderer::ClockRenderer(int w, int h) :
	iRenderer(w,h) {
	mParameters.insert("drawDial", false);
	mParameters.insert("scaleMode", 2);
	mParameters.insert("drawSecs", true);
	mParameters.insert("roundCaps", false);
	mParameters.insert("sizePercent", 100);
	mShowDigitalClock = true;
//	m_Parameters.insert("handWidth", 3);
//	m_Parameters.insert("lineWidth", 2);

}

QPoint pft(int r, double p) {
	double a = M_PI / 6.0f * p;
	double rr = r;
	return QPoint(sin(a) * rr, -cos(a) * rr);
}


//TODO: add background image support
void ClockRenderer::renderFrame(QPainter *painter, quint64 frameID)
{

	bool drawDial   = mParameters.value("drawDial", false).toBool();
	bool drawSeconds= mParameters.value("drawSecs", true).toBool();
	bool roundCaps  = mParameters.value("roundCaps", false).toBool();
	int  size       = mParameters.value("sizePercent", 100).toInt();
	int scaleMode   = mParameters.value("scaleMode", 0).toInt();
	int dia = qMin(mWidth, mHeight);
	dia *= size;
	dia /= 100;
	int lineWidth = mParameters.value("lineWidth", qMax(1, dia/144)).toInt();
	int handWidth = mParameters.value("handWidth", qMax(1, dia/144)*2).toInt();
	dia -=lineWidth*2;

	//draw an analog clock

	QColor handsColor(255,255,255);
	QColor scaleColor(192,192,192);
	QColor dialColor(200,200,200);

	int x0 = mWidth  / 2;
	int y0 = mHeight / 2;
	QPoint center(x0, y0);
	int r0 = dia/2;
	int r_hour = r0 * 11 / 16;
	int r_min = r0 *  7 / 8;
	int r_outer = r0 - lineWidth*4;
	int r_inner = r0 * 14/16;
	int r_axis =  3 * handWidth / 4;

	QRect dialRect(x0-dia/2,y0-dia/2,dia,dia);

	QPen dialPen(Qt::SolidLine);
	dialPen.setColor(dialColor);
	dialPen.setWidth(qMax(1, lineWidth * 2 / 3));

	QPen scalePen(Qt::SolidLine);
	scalePen.setColor(scaleColor);
	scalePen.setWidth(qMax(1, lineWidth * 3 / 2));


	QPen handsPen(Qt::SolidLine);
	handsPen.setColor(handsColor);
	handsPen.setWidth(qMax(1, handWidth));

	QPen secPen(Qt::SolidLine);
	secPen.setColor(Qt::red);
	secPen.setWidth(qMax(1, handWidth*3/4));


	if (roundCaps) {
		handsPen.setCapStyle(Qt::RoundCap);
		scalePen.setCapStyle(Qt::RoundCap);
		secPen.setCapStyle(Qt::RoundCap);
	}

	painter->setTransform(m_Transformation);
	painter->setRenderHint(QPainter::Antialiasing, true);
	if (!m_Background.isNull())
	{
		painter->drawPixmap(x0 - m_Background.width()/2, 0, m_Background);
		//p.fillRect(frame.rect(), QColor(0,0,64,64));
	}

	QPen pp = painter->pen();
	pp.setColor(Qt::white);
	painter->setPen(dialPen);
	if (drawDial   )
		painter->drawEllipse(dialRect);
	painter->setPen(scalePen);
	if (scaleMode) {
		int n=12, d=1;
		switch (scaleMode) {
			case 1:
				n=4;
				d = 3;
				break;
			case 2:
			default:
				break;
		}

		int h =0;
		for(int i=0;i<n;++i) {
			painter->drawLine(center + pft(r_inner, h),
						  center + pft(r_outer, h));
			h+=d;
		}
	}
	QTime now = QTime::currentTime();
	double h = now.hour();
	double m = now.minute();
	double s = now.second();
	m+=floor(s/15) /4;
	h+=m/60.0;
	m/=5.0;
	s/=5.0;
	painter->setPen(handsPen);
	painter->drawLine(center,
				  center + pft(r_hour, h));
	painter->drawLine(center,
				  center + pft(r_min, m));

	if (drawSeconds) {
		painter->setPen(secPen);
		painter->drawLine(center,
					  center + pft(r_min, s));
	}

	//draw axis cap
	painter->setBrush(painter->pen().color());
	painter->drawEllipse(center, r_axis, r_axis);

	int fontHeight = (mHeight - dia) / 6;
	fontHeight = qMin(mHeight / 10, fontHeight);
	if (mShowDigitalClock && (fontHeight>10))
	{
		QFont fnt("Freemono");
		fnt.setBold(true);
		fnt.setPixelSize(fontHeight);
		painter->setFont(fnt);
		QRect fontRect = QRect(0, mHeight/ 2 + dia/2 + fontHeight,
		                       mWidth, fontHeight * 1);
		QFontMetrics metrics(fnt);
		int flags = Qt::AlignCenter;
		QString text = now.toString("HH:mm:ss");
		fontRect = metrics.boundingRect(fontRect, flags, text);
		painter->setRenderHint(QPainter::TextAntialiasing, true);
		painter->setPen(Qt::white);
		painter->drawText(fontRect, flags, text);
	}
}

void ClockRenderer::setBackground(const QPixmap &bgnd)
{
	m_Background = bgnd;
}

void ClockRenderer::setRatio(const iRenderer::aspectRatio &Ratio)
{
//	iRenderer::setRatio(Ratio);

}
