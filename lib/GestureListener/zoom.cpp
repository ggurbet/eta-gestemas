
#include <QtCore/QtDebug>
#include "zoom.h"
#include "twotouchpinchgesturerecognizer.h"
#include "utilities.h"

void Zoom::setGestureRecognizer(TwoTouchPinchGestureRecognizer *recognizer)
{
    GestureListener::setGestureRecognizer(recognizer);
}

void Zoom::onBegan()
{
    qDebug() << "Zoom onBegan";
    m_counter = m_accumulator;
    m_averageScale = 0.0f;
}
void Zoom::onRecognized()
{
    qDebug() << "Zoom onRecognized";
}
void Zoom::onChanged()
{
    const TwoTouchPinchGestureRecognizer *p
        = static_cast<const TwoTouchPinchGestureRecognizer*>(m_recognizer);
    if (m_counter != 0) {
        m_averageScale += p->scale();
        --m_counter;
        return;
    }
    qDebug() << "Zoom onChanged";
    m_averageScale /= m_accumulator;
    qDebug() << "scale: " << m_averageScale;
    if (m_averageScale > 1.0f) {
        if (CHECK_RANGE(m_averageScale, minScale(), maxScale())) {
            qDebug() << "zoom out";
            zoomOut();
        }
    } else if (m_averageScale < 1.0f){
        if (CHECK_RANGE(m_averageScale, 1.0f/maxScale(), 1.0f/minScale())) {
            qDebug() << "zoom in";
            zoomIn();
        }
    }
    m_averageScale = 0.0f;
    m_counter = m_accumulator;
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
