#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"

#include <QtCore/QXmlStreamReader>
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QtDebug>

#include "rightclick.h"
#include "leftclick.h"
#include "zoom.h"

TargetFactory::TargetFactory()
    : m_configReader(nullptr),
      m_configFile(nullptr),
      m_currentTarget(nullptr)
{
    m_configReader = new QXmlStreamReader;
    Q_CHECK_PTR(m_configReader);
}

TargetFactory::TargetFactory(const QString& configFileName)
    : TargetFactory()
{
    setConfigurationFileName(configFileName);
}

TargetFactory::~TargetFactory()
{
    if (m_configFile && m_configFile->isOpen()) {
        m_configFile->close();
    }
    delete m_configReader;
    m_configFile->deleteLater();
    m_configReader = nullptr;
}

void TargetFactory::setConfigurationFileName(const QString& configFileName)
{
    m_configFile = new QFile(configFileName);
    Q_CHECK_PTR(m_configFile);
}

QString TargetFactory::configFileName() const
{
    if (!m_configFile) {
        return QString();
    }

    return m_configFile->fileName();
}

Target* TargetFactory::create(unsigned long targetId, const QString& targetName)
{
    Q_CHECK_PTR(m_configFile);

    m_currentTarget = nullptr;

    // Each root gesture should be added to each non-root window
    // instead  of grabbing touches on root window
    if (targetName == "root") {
        return nullptr;
    }

    bool success = false;
    success = m_configFile->open(QIODevice::ReadOnly);
    Q_ASSERT_X(success, "setConfigurationFileName", "Failed to open config file");
    m_configReader->setDevice(m_configFile);

        if (m_configReader->readNextStartElement()
            && m_configReader->name() == "Targets") {
            processTargets(targetName);
            if (m_configReader->tokenType() == QXmlStreamReader::Invalid) {
                m_configReader->readNext();
            }
            if (m_configReader->hasError()) {
                m_configReader->raiseError("Xml Configuration Reader Error.");
                qDebug() << m_configReader->errorString();
            }
        }
    m_configFile->close();
    if (m_currentTarget) {
        m_currentTarget->setTargetId(targetId);
        m_currentTarget->setTargetName(targetName);
        QList<GestureRecognizer*> gestureRecognizers =
            m_currentTarget->gestureRecognizers();
        if (gestureRecognizers.size() > 1) {
            QList<GestureRecognizer*> abortList;
            GestureRecognizer *gestureRecognizer = nullptr;
            int i = 0;
            foreach(gestureRecognizer, gestureRecognizers) {
                if (gestureRecognizer->allowsSimultaneousRecognition()) {
                    // Abort gestures which don't allow simultaneous recognition
                    for (i = 0; i < gestureRecognizers.size(); ++i) {
                        if (!gestureRecognizers[i]->allowsSimultaneousRecognition()) {
                            abortList.append(gestureRecognizers[i]);
                        }
                    }
                } else {
                    // Abort all gestures except for itself
                    abortList = gestureRecognizers;
                    abortList.removeAll(gestureRecognizer);
                }
                gestureRecognizer->setGestureRecognizersToAbort(abortList);
            }
        }
    }
    return m_currentTarget;
}

void TargetFactory::processTargets(const QString& targetName)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Targets") {
        return;
    }
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "Target") {
            processTarget(targetName);
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processTarget(const QString& targetName)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Target") {
        return;
    }

    QString name;
    bool match = false;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "name") {
            name = m_configReader->readElementText();
            if (name == targetName || name == "root") {
                if (!m_currentTarget) {
                    m_currentTarget = new Target;
                    Q_CHECK_PTR(m_currentTarget);
                }
                match = true;
            }
        } else if (match && m_configReader->name() == "GestureRecognizers") {
            processGestureRecognizers();
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processGestureRecognizers()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "GestureRecognizers") {
        return;
    }

    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "LongPress") {
            processLongPress();
        } else if (m_configReader->name() == "TwoTouchPinch") {
            processTwoTouchPinch();
        } else if (m_configReader->name() == "Pan") {
            processPan();
        } else if (m_configReader->name() == "Tap") {
            processTap();
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processLongPress()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "LongPress") {
        return;
    }

    bool ok = false;
    int numTouchesRequired = 0;
    int minPressDuration = 0;
    float maxAllowableDrift = 0.0f;
    bool allowSimultaneousRecognition = false;
    LongPressGestureRecognizer *gr = new LongPressGestureRecognizer;
    Q_CHECK_PTR(gr);
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "numTouchesRequired") {
            numTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTouchesRequired(numTouchesRequired);
            }
        } else if (m_configReader->name() == "minPressDuration") {
            minPressDuration =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMinPressDuration(minPressDuration);
            }
        } else if (m_configReader->name() == "maxAllowableDrift") {
            maxAllowableDrift =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxAllowableDrift(maxAllowableDrift);
            }
        } else if (m_configReader->name() == "allowSimultaneousRecognition") {
            allowSimultaneousRecognition =
                m_configReader->readElementText() == "true" ? true : false;
            gr->setAllowSimultaneousRecognition(allowSimultaneousRecognition);
        } else if (m_configReader->name() == "gestureListener") {
            QString listenerName = m_configReader->readElementText();
            if (listenerName == "RightClick") {
                RightClick *listener = new RightClick;
                listener->setGestureRecognizer(gr);
            }
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processPan()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Pan") {
        return;
    }

    bool ok = false;
    int maxNumTouchesRequired = 0;
    int minNumTouchesRequired = 0;
    float maxVelocity = 0.0f;
    float minVelocity = 0.0f;
    float maxAllowableDrift = 0.0f;
    bool allowSimultaneousRecognition = false;
    PanGestureRecognizer *gr = new PanGestureRecognizer;
    Q_CHECK_PTR(gr);
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "maxNumTouchesRequired") {
            maxNumTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxNumTouchesRequired(maxNumTouchesRequired);
            }
        } else if (m_configReader->name() == "minNumTouchesRequired") {
            minNumTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMinNumTouchesRequired(minNumTouchesRequired);
            }
        } else if (m_configReader->name() == "maxVelocity") {
            maxVelocity = m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxVelocity(maxVelocity);
            }
        } else if (m_configReader->name() == "minVelocity") {
            minVelocity = m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMinVelocity(minVelocity);
            }
        } else if (m_configReader->name() == "maxAllowableDrift") {
            maxAllowableDrift =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxAllowableDrift(maxAllowableDrift);
            }
        } else if (m_configReader->name() == "allowSimultaneousRecognition") {
            allowSimultaneousRecognition =
                m_configReader->readElementText() == "true" ? true : false;
            gr->setAllowSimultaneousRecognition(allowSimultaneousRecognition);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processTwoTouchPinch()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "TwoTouchPinch") {
        return;
    }

    bool ok = false;
    float maxScale = 0.0f;
    float minScale = 0.0f;
    float maxAllowableDrift = 0.0f;
    bool allowSimultaneousRecognition = false;
    TwoTouchPinchGestureRecognizer * gr =
        new TwoTouchPinchGestureRecognizer;
    Q_CHECK_PTR(gr);
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "maxScale") {
            maxScale = m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxScale(maxScale);
            }
        } else if (m_configReader->name() == "minScale") {
            minScale = m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMinScale(minScale);
            }
        } else if (m_configReader->name() == "maxAllowableDrift") {
            maxAllowableDrift =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxAllowableDrift(maxAllowableDrift);
            }
        } else if (m_configReader->name() == "allowSimultaneousRecognition") {
            allowSimultaneousRecognition =
                m_configReader->readElementText() == "true" ? true : false;
            gr->setAllowSimultaneousRecognition(allowSimultaneousRecognition);
        } else if (m_configReader->name() == "gestureListener") {
            QString listenerName = m_configReader->readElementText();
            if (listenerName == "Zoom") {
                Zoom *listener = new Zoom;
                listener->setGestureRecognizer(gr);
            }
        } else if (m_configReader->name() == "accumulator") {
            int accumulator =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setAccumulator(accumulator);
            }
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processTap()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Tap") {
        return;
    }

    bool ok = false;
    int numTouchesRequired = 0;
    int numTapsRequired = 0;
    int maxTapDelay = 0;
    int maxTapDuration = 0;
    float maxTapDistance = 0.0f;
    float maxAllowableDrift = 0.0f;
    bool allowSimultaneousRecognition = false;
    TapGestureRecognizer *gr = new TapGestureRecognizer;
    Q_CHECK_PTR(gr);
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "numTouchesRequired") {
            numTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTouchesRequired(numTouchesRequired);
            }
        } else if(m_configReader->name() == "numTapsRequired") {
            numTapsRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTapsRequired(numTapsRequired);
            }
        } else if(m_configReader->name() == "maxTapDelay") {
            maxTapDelay =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxTapDelay(maxTapDelay);
            }
        } else if(m_configReader->name() == "maxTapDuration") {
            maxTapDuration =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxTapDuration(maxTapDuration);
            }
        } else if(m_configReader->name() == "maxTapDistance") {
            maxTapDistance =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxTapDistance(maxTapDistance);
            }
        } else if (m_configReader->name() == "maxAllowableDrift") {
            maxAllowableDrift =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxAllowableDrift(maxAllowableDrift);
            }
        } else if (m_configReader->name() == "allowSimultaneousRecognition") {
            allowSimultaneousRecognition =
                m_configReader->readElementText() == "true" ? true : false;
            gr->setAllowSimultaneousRecognition(allowSimultaneousRecognition);
        } else if (m_configReader->name() == "gestureListener") {
            QString listenerName = m_configReader->readElementText();
            if (listenerName == "LeftClick") {
                LeftClick *listener = new LeftClick;
                listener->setGestureRecognizer(gr);
            }
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}
