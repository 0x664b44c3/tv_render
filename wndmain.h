#ifndef WNDMAIN_H
#define WNDMAIN_H
#include <clockrenderer.h>
#include <nukerenderer.h>
#include <QMainWindow>
#include <QLabel>
#include <QElapsedTimer>

namespace Ui {
class wndMain;
}

class wndMain : public QMainWindow
{
	Q_OBJECT

public:
	explicit wndMain(QWidget *parent = 0);
	~wndMain();

private:
	Ui::wndMain *ui;
	QLabel lbl;
	QElapsedTimer m_tmr;
	QList<QPixmap> mBackgroundVideo;
	QList<iRenderer*> mRenderQueue;
	int mBackgroundFrame;
	int mFrameWidth;
	int mFrameHeight;
private slots:
	void on_Update();
};

#endif // WNDMAIN_H
