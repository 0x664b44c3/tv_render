#include "wndmain.h"
#include "ui_wndmain.h"
#include <clockrenderer.h>
#include <nukerenderer.h>
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QImage>
#include <marquee.h>
#include <QFile>
#include <fahrplan.h>
#include <QCursor>
wndMain::wndMain(QWidget *parent) :
   QMainWindow(parent),
	ui(new Ui::wndMain) {
	ui->setupUi(this);
	mFrameWidth = 1280;
	mFrameHeight = 720 ;

//	bgnd = bgnd.scaledToHeight(576*2);
	iRenderer * renderer = new ClockRenderer(mFrameWidth / 2, mFrameHeight);
	renderer->setParameter("sizePercent", 75);
	QTransform transform;
	transform.scale(1, 1);
	transform.translate(mFrameWidth / 2, 0);
	renderer->setTransform(transform);
	mRenderQueue.push_back(renderer);

	transform.reset();
	renderer = new Marquee(mFrameWidth, mFrameHeight);

	renderer->setOpacity(192);
	renderer->setTransform(transform);
//	mRenderQueue.push_back(renderer);
	QTimer * t = new QTimer(this);
	t->start(120);
	connect(t, SIGNAL(timeout()), SLOT(on_Update()));
	lbl.show();//FullScreen();
	lbl.setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	lbl.setMinimumSize(mFrameWidth, mFrameHeight);
	//load background
	Fahrplan * fp = new Fahrplan(mFrameWidth / 2, mFrameHeight);
	QFile scheduleFile("/home/andreas/talks.json");
	if (scheduleFile.open(QFile::ReadOnly))
	{
		fp->loadFahrplan(scheduleFile.readAll());

	}
	mRenderQueue.push_front(fp);
	int i=0;
	QString filePath="/home/andreas/Desktop/Aktuelles/2020-12-05/frames/";
	while (0)
	{
		QString name;
		name.sprintf("frame%02d.png", ++i);
		qDebug()<<"Loading frame"<<i<<filePath+name;
		QPixmap nextFrame(filePath+name);
		if (nextFrame.isNull())
			break;
		mBackgroundVideo.append(nextFrame);
	}
//	qDebug()<<
	mBackgroundFrame = 0;
	m_tmr.start();
	lbl.setWindowTitle("TV Render");
	QCursor none;
	none.setShape(Qt::ForbiddenCursor);
	lbl.setCursor(none);
}

wndMain::~wndMain()
{
	delete ui;
}

void wndMain::on_Update()
{
	static int field=0;
	field = 1-field;
//	QPixmap pic = m_clock.renderFrame((int)(m_tmr.elapsed() / 100 ));
//		if ((pic.width()!=mFrameWidth)
//		 || (pic.height()!=mFrameHeight)) {
//		m_clock = ClockRenderer(768, 576);
//		//QPixmap bgnd("/home/andreas/Desktop/base.jpg");
//		//bgnd = bgnd.scaledToHeight(mFrameHeight*2);
//		//m_clock.setBackground(bgnd);
//	}

	if(mBackgroundFrame >= mBackgroundVideo.size())
		mBackgroundFrame = 0;
	QImage img(mFrameWidth, mFrameHeight, QImage::Format_ARGB32_Premultiplied);

	QRect safeArea;
	safeArea.setLeft(img.width() * 5 / 100);
	safeArea.setTop(img.height() * 5 / 100);
	safeArea.setWidth( img.width() * 90 / 100);
	safeArea.setHeight(img.height() * 90 / 100);
	img.fill(Qt::black);
	{
		QPainter p(&img);

		if (false)
			p.drawPixmap(0, 0, mBackgroundVideo[mBackgroundFrame++]);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.setRenderHint(QPainter::TextAntialiasing, true);
		foreach(iRenderer * e, mRenderQueue)
		{
			e->renderFrame(&p, 0);
			p.setOpacity(e->opacity());
		}
		p.setPen(Qt::green);
		p.setTransform(QTransform());
		p.setBrush(Qt::NoBrush);
//		p.drawRect(safeArea);
	}

//	img = img.scaled(768, 576, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//	ui->label->setPixmap(QPixmap::fromImage(img));
	//m_clock.setBackground(pic);
	lbl.setPixmap(QPixmap::fromImage(img));
//	float ratio_w = ((float) mFrameWidth) / ((float) pic.width());
//	float ratio_h = ((float) mFrameHeight) / ((float) pic.height());
//	if (ratio_w>ratio_h) { //width is larger scale - scale height
//		lbl.setPixmap(pic.scaledToHeight(mFrameHeight));
//	}else {
//		lbl.setPixmap(pic.scaledToWidth(mFrameWidth));
//	}

	//lbl.setPixmap(pic);
	//pic.save("/home/andreas/nukular.png");

//	QPainter p(&pic);
//	QPen pp = p.pen();
//	pp.setWidth(2);
//	pp.setColor(QColor(0,0,0,64));
//	p.setPen(pp);
//	int v=field-1;
//	for ( int l = field;l<pic.height();l+=4) {
//		v+=1;
//		p.drawLine(0,l,pic.width(),l);
//		//p.drawPixmap(0,l,pic,0,v,pic.width(),1);
//	}
//	p.end();
//	//QPixmap scaled = pic.scaledToHeight(ui->label->height());
	//ui->label->setPixmap(pic);
}
