
#include "xtesttwotouchpinchzoom.h"
#include "twotouchpinchgesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestTwoTouchPinchZoom::XTestTwoTouchPinchZoom()
    :m_zoomInShortcut(nullptr),
     m_zoomOutShortcut(nullptr)
{
}

 XTestTwoTouchPinchZoom::~XTestTwoTouchPinchZoom()
{
    delete m_zoomInShortcut;
    delete m_zoomOutShortcut;
    m_zoomInShortcut = nullptr;
    m_zoomOutShortcut = nullptr;
}

void XTestTwoTouchPinchZoom::setZoomInShortcut(
                            const XTestShortcut *zoomInShortcut)
{
    m_zoomInShortcut = zoomInShortcut;
}

void XTestTwoTouchPinchZoom::setZoomOutShortcut(
                            const XTestShortcut *zoomOutShortcut)
{
    m_zoomOutShortcut = zoomOutShortcut;
}

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
    m_zoomInShortcut->press();
    m_zoomInShortcut->release();
}

void XTestTwoTouchPinchZoom::zoomOut()
{
    m_zoomOutShortcut->press();
    m_zoomOutShortcut->release();
}

bool XTestTwoTouchPinchZoom::isEqual(const GestureListener& other) const
{
    const XTestTwoTouchPinchZoom *p = static_cast<const XTestTwoTouchPinchZoom*>(&other);
    if (m_accumulator != p->m_accumulator) return false;
    if (m_maxScale != p->m_maxScale) return false;
    if (m_minScale != p->m_minScale) return false;

    return true;
}
