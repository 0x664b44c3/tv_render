#include "nukerenderer.h"
#include <QPainter>
#include <QPainterPath>
#include <math.h>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
NukeRenderer::NukeRenderer(int w, int h) :
	iRenderer(w, h)
{
//	m_MediaObject = new Phonon::MediaObject();
//	m_AudioDev = new Phonon::AudioOutput(Phonon::NotificationCategory);
//	m_MediaObject->setCurrentSource(QString("/home/andreas/beep.wav"));
//	Phonon::createPath(m_MediaObject, m_AudioDev);
	m_Frame = 0;
	m_tmr.restart();
}

void NukeRenderer::renderFrame(QPainter * p, quint64 frameID)
{

	m_Frame++;
	static int ttx=0;
	quint64 tt = frameID * 33;
	tt/=1000;
	int tmax = 360;
	int tl = tmax - tt;
	int tm = tl / 60;
	int ts = tl % 60;
	int tf = (999 - (frameID * 33 % 1000))/10;

	if (ts != ttx) {
		ttx = ts;
		//m_MediaObject->play();
	}
//		QStringList fileList;
//		fileList << "/home/andreas/beep.wav";
//		QProcess::("/usr/bin/mplayer", fileList);
//	}
	//Debug()<<"render"<< m_Frame;
//	QPixmap pxm(m_Width, m_Height);
//	QPainter painter(&pxm);
//	pxm.fill(Qt::black);
//	renderNuke(&painter);
//	int w = m_Width / 2.5;
//	int h = m_Height / 6;
//	QRect clockRect(m_Width / 2 - w / 2, m_Height / 2 - h / 2, w, h);
//	clockRect.moveBottom(m_Height);
//	painter.setPen(Qt::NoPen);
//	painter.setBrush(QColor(0,0,0,128));
//	//painter.drawRect(clockRect );
//	QFont font("sans");
//	font.setPixelSize(h * 7 / 8);
//	painter.setFont(font);
//	QFontMetrics m = painter.fontMetrics();
//	QString clockText;
//	//clockText.sprintf("%02d:%02d:%02d", tm, ts, tf);
//	clockText = QDateTime::currentDateTime().toString("hh:mm:ss");
//	QRect fontRect = m.boundingRect(clockRect, Qt::AlignCenter, clockText);
//	//fontRect.moveTopLeft(clockRect.topLeft());
//	painter.setPen(Qt::white);
//	//painter.drawText(fontRect, clockText);
//	m_Frame++;
//	painter.end();
//	return pxm;
	renderNuke(p);

}

QPointF arc2carthesian(QPointF c, float r, float angle) {
	QPointF p;
	p.setX(c.x() + cos(angle * M_PI / 180.0) * r);
	p.setY(c.y() - sin(angle * M_PI / 180.0) * r);
	return p;
}

void NukeRenderer::renderNuke(QPainter * p) const
{
	QColor fg = Qt::black;
	QColor bg = Qt::yellow;
	if ((m_Frame & 0x20) == 0) {
		fg = Qt::red;
		bg = Qt::yellow;
	}
	p->setRenderHint(QPainter::Antialiasing, true);

	p->fillRect(QRect(0,0,mWidth, mHeight), QBrush(bg));
	int nukeRadius = qMin(mWidth, mHeight)  / 2;
	int innerRadius = nukeRadius / 5;
	int gap = innerRadius / 4;
	int cx = mWidth  / 2;
	int cy = mHeight / 2;
	QRect irect(cx-innerRadius / 2, cy-innerRadius / 2, innerRadius, innerRadius);
	QBrush fillBrush(fg);
	p->setPen(Qt::NoPen);
	p->setBrush(fillBrush);
	p->drawEllipse(irect);
	int r2 = innerRadius + gap;

	QPainterPath path;

	QRectF rct0(cx - r2/2, cy - r2/2, r2, r2);
	int nr = nukeRadius - gap / 2;
	QRectF rct1(cx - nr / 2, cy - nr / 2, nr , nr);
	QPointF c0(cx, cy);
	int rotation=30;//30+m_Frame%360;
	path.arcMoveTo(rct0, rotation - 30);
	path.arcTo(rct0, rotation - 30, 60 );
	path.arcTo(rct1, rotation + 30, -60 );
	path.closeSubpath();
	rotation+=120;
	path.arcMoveTo(rct0, rotation - 30);
	path.arcTo(rct0, rotation - 30, 60 );
	path.arcTo(rct1, rotation + 30, -60 );
	path.closeSubpath();
	rotation+=120;
	path.arcMoveTo(rct0, rotation - 30);
	path.arcTo(rct0, rotation - 30, 60 );
	path.arcTo(rct1, rotation + 30, -60 );
	path.closeSubpath();


	p->drawPath(path);
	QPen pp = p->pen();
	pp.setColor(fg);
	pp.setStyle(Qt::SolidLine);
	pp.setCapStyle(Qt::FlatCap);
	pp.setWidth(gap / 2);
	p->setPen(pp);
	for(int z0 = 1; z0 < 16; ++z0) {
		float z = z0 - ((float)(m_Frame %30 )) / 10.0;
		float rx0 = (nukeRadius + gap) / 2;
		float r3 = sqrt(mWidth * mWidth + mHeight * mHeight)/2;
		float r4 = r3- r3/10 / sqrt(z);

		r3/=sqrt(z);
		r4/=sqrt(z);

		r3 = qMax(rx0, r3);
		r4 = qMax(rx0, r4);
		if (fabs(r3 - r4)<1)
			continue;

		for (int seg=0;seg<24;++seg) {
			float alpha = seg * 360 / 24;
			QPointF p1 = arc2carthesian(c0, r3, alpha);
			QPointF p2 = arc2carthesian(c0, r4, alpha);
			p->drawLine(p1, p2);
		}
	}
		float r3 = (nukeRadius + gap) / 2;
		float r4 = r3 + gap ;

		for (int seg=0;seg<24;++seg) {
			float alpha = seg * 360 / 24;
			QPointF p1 = arc2carthesian(c0, r3, alpha);
			QPointF p2 = arc2carthesian(c0, r4, alpha);
			p->drawLine(p1, p2);
		}

}
