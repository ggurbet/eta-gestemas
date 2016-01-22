
#include <QtCore/QtDebug>
#include "xtesttwotouchpinchzoom.h"
#include "twotouchpinchgesturerecognizer.h"
#include "xtest.h"
#include "utilities.h"

void XTestTwoTouchPinchZoom::onBegan()
{
    m_counter = m_accumulator;
    m_averageScale = 0.0f;
}

void XTestTwoTouchPinchZoom::onRecognized()
{
}

void XTestTwoTouchPinchZoom::onChanged()
{
    const TwoTouchPinchGestureRecognizer *p = m_recognizer;
    if (m_counter != 0) {
        m_averageScale += p->scale();
        --m_counter;
        return;
    }
    m_averageScale /= m_accumulator;
    if (m_averageScale > 1.0f) {
        if (CHECK_RANGE(m_averageScale, minScale(), maxScale())) {
            zoomOut();
        }
    } else if (m_averageScale < 1.0f){
        if (CHECK_RANGE(m_averageScale, 1.0f/maxScale(), 1.0f/minScale())) {
            zoomIn();
        }
    }
    m_averageScale = 0.0f;
    m_counter = m_accumulator;
}

void XTestTwoTouchPinchZoom::onCanceled()
{
}

void XTestTwoTouchPinchZoom::onEnded()
{
}

void XTestTwoTouchPinchZoom::onFailed()
{
}

void XTestTwoTouchPinchZoom::zoomIn()
{
    const char* modifiers[] = {"Control_L", ""};
    XTest::injectButton(5, modifiers);
}

void XTestTwoTouchPinchZoom::zoomOut()
{
    const char* modifiers[] = {"Control_L", ""};
    XTest::injectButton(4, modifiers);
}

bool XTestTwoTouchPinchZoom::isEqual(const GestureListener& other) const
{
    const XTestTwoTouchPinchZoom *p = static_cast<const XTestTwoTouchPinchZoom*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxScale != p->m_maxScale) return false;
    if (m_minScale != p->m_minScale) return false;

    return true;
}
