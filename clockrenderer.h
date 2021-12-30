#ifndef CLOCKRENDERER_H
#define CLOCKRENDERER_H
#include <irenderer.h>
#include <QObject>

/**
 * @brief The ClockRenderer class
 * This class renders a simple analog clock.
 * Bear in mind, that this code assumes pixel aspect ration of 1:1
 */
class ClockRenderer : public iRenderer {
public:
	explicit ClockRenderer(int width, int height);
	virtual void renderFrame(QPainter *, quint64 frameID);
	void setBackground(const QPixmap & bgnd);
	void setRatio(const aspectRatio &Ratio);
signals:

public slots:

private:
	QPixmap m_Background;
	bool mShowDigitalClock;
};

#endif // CLOCKRENDERER_H
