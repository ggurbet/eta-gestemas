
#include <QtCore/QtDebug>
#include "zoom.h"
#include "twotouchpinchgesturerecognizer.h"

void Zoom::setGestureRecognizer(TwoTouchPinchGestureRecognizer *recognizer)
{
    GestureListener::setGestureRecognizer(recognizer);
}

void Zoom::onBegan()
{
    qDebug() << "Zoom onBegan";
}
void Zoom::onRecognized()
{
    qDebug() << "Zoom onRecognized";
}
void Zoom::onChanged()
{
    qDebug() << "Zoom onChanged";
    const TwoTouchPinchGestureRecognizer *p
        = static_cast<const TwoTouchPinchGestureRecognizer*>(m_recognizer);
    float scale = p->scale();
    if (scale >= 1.0f) {
        qDebug() << "zoom out: " << scale;
        const char* modifiers[] = {"Control_L", ""};
        injectButton(4, modifiers);
    } else {
        qDebug() << "zoom in: " << scale;
        const char* modifiers[] = {"Control_L", ""};
        injectButton(5, modifiers);
    }
}
void Zoom::onCanceled()
{
    qDebug() << "Zoom onCanceled";
}
void Zoom::onEnded()
{
    qDebug() << "Zoom onEnded";
}
void Zoom::onFailed()
{
    qDebug() << "Zoom onFailed";
}
