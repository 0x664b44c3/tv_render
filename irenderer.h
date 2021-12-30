/**
  Renderer interface class
*/
#ifndef IRENDERER_H
#define IRENDERER_H
#include <QVariantMap>
#include <QPixmap>
#include <tvtypes.h>

class QPainter;

class iRenderer
{
public:
	struct aspectRatio {
		int w;
		int h;
	};
	explicit iRenderer(int width, int height) {
		mWidth = width;
		mHeight = height;
		m_Transformation.reset();
		mOpacity = 255;
	}
	virtual QVariantMap paremeters() {
		return mParameters;
	}
	virtual void setParameters(const QVariantMap & params) {
		mParameters = params;
	}
	virtual bool setParameter(QString param, QVariant val)
	{
		if (!mParameters.contains(param))
			return false;
		mParameters[param] = val;
		return true;
	}
	virtual QVariant parameter(QString param)
	{
		return mParameters.value(param, QVariant());
	}
	virtual void renderFrame(QPainter *, quint64 frameID=0) = 0;
	virtual fps_fract framerate() const {
		return m_Framerate;
	}
	virtual void setFramerate(const fps_fract & fps) {
		m_Framerate = fps;
	}
	virtual void setTransform(const QTransform & t)
	{
		m_Transformation = t;
	}
	virtual int opacity() const
	{
		return mOpacity;
	}
	virtual void setOpacity(int o)
	{
		mOpacity = o;
	}

//	virtual aspectRatio Ratio() const;
//	virtual void setRatio(const aspectRatio &Ratio);

protected:
	QVariantMap mParameters;
	fps_fract m_Framerate;
	QTransform m_Transformation;
	int mOpacity;
	int mWidth;
	int mHeight;
//	aspectRatio m_Aspect;
};

#endif // IRENDERER_H

//iRenderer::aspectRatio iRenderer::Ratio() const
//{
//	return m_Aspect;
//}

//void iRenderer::setRatio(const iRenderer::aspectRatio &Ratio)
//{
//	m_Aspect = Ratio;
//}

