
#include "xtestzoom.h"
#include "pinchgesturerecognizer.h"
#include "xtestshortcut.h"
#include "utilities.h"
#include <QtCore/QtDebug>

XTestZoom::XTestZoom()
    :m_minEffectiveVelocity(MIN_EFFECTIVE_VELOCITY),
     m_recognizer(nullptr),
     m_zoomInShortcut(nullptr),
     m_zoomOutShortcut(nullptr)
{
}

 XTestZoom::~XTestZoom()
{
    delete m_zoomInShortcut;
    delete m_zoomOutShortcut;
    m_zoomInShortcut = nullptr;
    m_zoomOutShortcut = nullptr;
}

void XTestZoom::setGestureRecognizer(const PinchGestureRecognizer *recognizer)
{
    m_recognizer = recognizer;
}

void XTestZoom::setZoomInShortcut(
                            const XTestShortcut *zoomInShortcut)
{
    m_zoomInShortcut = zoomInShortcut;
}

void XTestZoom::setZoomOutShortcut(
                            const XTestShortcut *zoomOutShortcut)
{
    m_zoomOutShortcut = zoomOutShortcut;
}

void XTestZoom::onBegan()
{
}

void XTestZoom::onRecognized()
{
}

void XTestZoom::onChanged()
{
    float velocity = m_recognizer->velocity();
    if (velocity > m_minEffectiveVelocity) {
            zoomOut();
    } else if (velocity < -m_minEffectiveVelocity){
            zoomIn();
    }
}

void XTestZoom::onCanceled()
{
}

void XTestZoom::onEnded()
{
}

void XTestZoom::onFailed()
{
}

void XTestZoom::zoomIn()
{
    m_zoomInShortcut->press();
    m_zoomInShortcut->release();
}

void XTestZoom::zoomOut()
{
    m_zoomOutShortcut->press();
    m_zoomOutShortcut->release();
}

bool XTestZoom::isEqual(const GestureListener& other) const
{
    const XTestZoom *p = static_cast<const XTestZoom *>(&other);
    if (m_minEffectiveVelocity != p->m_minEffectiveVelocity) return false;

    return true;
}
