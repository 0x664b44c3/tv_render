#include "marquee.h"
#include<QDebug>

Marquee::Marquee(int w, int h, QObject *parent) : iRenderer(w, h)//, QObject(parent)
{
	int bandHeight = 0;
	int fontHeight=32;
	mParameters.insert("y", h*85/100);
	mParameters.insert("x0", 0);
	mParameters.insert("height", bandHeight = h * 8 / 100);
	mParameters.insert("fontSize", fontHeight = h*6/100);
	mParameters.insert("font", "verdana");
	mParameters.insert("spacing", w / 50);
	mParameters.insert("speed", 3);
	mSpacing = 10;

//	m_Transformation.scale(720.0/768.0, 1);
	QFont f = QFont("arial narrow");
	f.setPixelSize(fontHeight);
	f.setBold(false);
	textElement * breaking = new textElement("UP NEXT", f, bandHeight, Qt::red);
	f.setBold(false);
	textElement * news     = new textElement("Wetterballon startet heute um 22:30 Uhr am Sandplatz", f, bandHeight);
	//f.setBold(true);
	textElement * vortrag  = new textElement(QString::fromUtf8("Nächster Vortrag:"), f, bandHeight);
	f.setBold(false);
	textElement * te = new textElement("Tiefbrunnen bauen", f, bandHeight);
	//f.setPixelSize(24);
	textElement * splitter = new textElement("+++", f, bandHeight);
	te->paint(m_Transformation);
	splitter->paint(m_Transformation);
	mQueue.push_back(breaking);
	mQueue.push_back(news);
	mQueue.push_back(splitter);
	mQueue.push_back(vortrag);
	mQueue.push_back(te);
	te = new textElement("Aktueller Stromverbrauch: 28,5kW", f, bandHeight);
	te->paint(m_Transformation);
	splitter->addRef();
	mQueue.push_back(splitter);
	mQueue.push_back(te);
	splitter->addRef();
	mQueue.push_back(splitter);

	popNextElement();
}

Marquee::~Marquee()
{

}

void Marquee::pushElement(iMessageElement * e)
{
	mQueue.push_back(e);
}

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
void Marquee::renderFrame(QPainter * p, quint64 frameID)
{

	int y       = mParameters.value("y", mHeight*8/10).toInt();
	int x0      = mParameters.value("x0", 0).toInt();
	int height  = mParameters.value("height", mHeight/10).toInt();
	mSpacing    = mParameters.value("spacing", mWidth / 50).toInt();
	mSpeed      = mParameters.value("speed", 3).toInt();

	QTransform otx = p->transform();
	QTransform unity;
	unity.reset();
	p->setTransform(unity);
	p->fillRect(x0, y, mWidth, height, QColor(0,0,0,128));
	for(auto it = mDisplayList.begin(); it!=mDisplayList.end(); ++it)
	{
		p->drawImage(it->x, y, it->element->getImage());
		it->x-=mSpeed;
	}
	p->setTransform(otx);
	//check if first element is out of screen
	if (mDisplayList.isEmpty())
		return;
	if ( mDisplayList.first().x < -mDisplayList.first().element->width() )
	{
		mDisplayList.first().element->release();
		mDisplayList.takeFirst();
	}
	//check if space is behind last element on the right
	if (mDisplayList.isEmpty())
	{
		popNextElement();
		return;
	}
	if (mDisplayList.last().x < (mWidth + mDisplayList.last().element->width() + mSpacing*2 + mSpeed*2))
		popNextElement();

}

void Marquee::popNextElement()
{
	if (mQueue.isEmpty())
		return;
	int x0 = mWidth;
	if (mDisplayList.size())
		x0 = mDisplayList.last().x + mDisplayList.last().element->width();
	x0 = qMax(mWidth, x0);
	iMessageElement * me = mQueue.takeFirst();
	if (me->getImage().isNull())
		me->paint(m_Transformation);
	activeEntry ae(me, x0 + mSpacing);
	if (me->reQueue())
	{
		me->addRef();
		mQueue.push_back(me);
	}

	mDisplayList.push_back(ae);
}
