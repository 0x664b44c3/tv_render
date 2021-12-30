#pragma once
#ifndef MARQUEERENDERER_H
#define MARQUEERENDERER_H

#include <irenderer.h>

class QPainter;



class MarqueeRenderer : public iRenderer
{
	public:
		explicit MarqueeRenderer(int width, int height);
		void renderFrame(QImage &, quint64 frameID) const;

	private:
		QImage bgImage;
};

#endif // MARQUEERENDERER_H
