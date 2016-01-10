
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
        zoomOut();
    } else {
        qDebug() << "zoom in: " << scale;
        zoomIn();
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

void Zoom::zoomIn()
{
    const char* modifiers[] = {"Control_L", ""};
    injectButton(5, modifiers);
}

void Zoom::zoomOut()
{
    const char* modifiers[] = {"Control_L", ""};
    injectButton(4, modifiers);
}

bool Zoom::isEqual(const GestureListener& other) const
{
    const Zoom *p = static_cast<const Zoom*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxScale != p->m_maxScale) return false;
    if (m_minScale != p->m_minScale) return false;

    return true;
}
