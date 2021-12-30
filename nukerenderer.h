#ifndef NUKERENDERER_H
#define NUKERENDERER_H
#include <irenderer.h>
#include <QElapsedTimer>
#include <QSound>
//#include <phonon/AudioOutput>
//#include <phonon/MediaObject>

class QPainter;
class NukeRenderer : public iRenderer
{

public:
	explicit NukeRenderer(int w, int h);
	~NukeRenderer();
	void renderFrame(QPainter * , quint64 frameID);
signals:

public slots:
private:
	void renderNuke(QPainter*) const;
	mutable int m_Frame;
	mutable QElapsedTimer m_tmr;
//	mutable Phonon::MediaObject * m_MediaObject;
//	Phonon::AudioOutput *m_AudioDev;
	//mutable QSound m_Beep;
};

#endif // NUKERENDERER_H
