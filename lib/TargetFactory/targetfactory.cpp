#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"
#include "swipegesturerecognizer.h"

#include <QtCore/QXmlStreamReader>
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QtDebug>

#include "xtest.h"
#include "xtestshortcut.h"
#include "xtestlongpressmove.h"
#include "xtestlongpressrightclick.h"
#include "xtestpanmove.h"
#include "xtestpanscroll.h"
#include "xtesttaprightclick.h"
#include "xtesttwotouchpinchzoom.h"
#include "xtestswipeswitch.h"
#include "dbusswipekeyboard.h"

TargetFactory::TargetFactory()
    : m_configReader(nullptr),
      m_configFile(nullptr),
      m_currentTarget(nullptr),
      m_parsedTarget(UndefinedTarget)
{
    m_configReader = new QXmlStreamReader;
    Q_CHECK_PTR(m_configReader);
    XTest::open();
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
    XTest::close();
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
    m_parsedTarget = UndefinedTarget;

    if (targetName.isEmpty()) {
        // This could also be the root
        return nullptr;
    }

    bool success = false;
    success = m_configFile->open(QIODevice::ReadOnly);
    Q_ASSERT_X(success, "setConfigurationFileName", "Failed to open config file");
    m_configReader->setDevice(m_configFile);
    if (m_configReader->readNextStartElement()
        && m_configReader->name() == "gestemas") {
        processAll(targetName);
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
        if (gestureRecognizers.size() == 0) {
            delete m_currentTarget;
            m_currentTarget = nullptr;
        } else if (gestureRecognizers.size() > 1) {
            QList<GestureRecognizer*> abortList;
            GestureRecognizer *gestureRecognizer = nullptr;
            int i = 0;
            int gestureId = 0;
            int abortId = 0;
            foreach(gestureRecognizer, gestureRecognizers) {
                if (gestureRecognizer->allowsSimultaneousRecognition()) {
                    gestureId = gestureRecognizer->id();
                    // Abort gestures given in config file
                    for (i = 0; i < gestureRecognizers.size(); ++i) {
                        abortId = gestureRecognizers[i]->id();
                        if (m_abortTable[gestureId].contains(abortId)) {
                            abortList.append(gestureRecognizers[i]);
                        }
                    }
                } else { // no simultaneous gestures
                    // Abort all gestures except for itself
                    abortList = gestureRecognizers;
                    abortList.removeAll(gestureRecognizer);
                }
                gestureRecognizer->setGestureRecognizersToAbort(abortList);
                abortList.clear();
            }
        }
    }
    m_abortTable.clear();
    return m_currentTarget;
}

void TargetFactory::processAll(const QString& targetName)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "gestemas") {
        return;
    }
    bool ok = false;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "GlobalTarget") {
            if (m_parsedTarget != PrivateTarget) {
                processGlobalTarget();
            } else {
                m_configReader->skipCurrentElement();
            }
        } else if (m_configReader->name() == "PublicTarget") {
            if (m_parsedTarget != PrivateTarget) {
                processPublicTarget(targetName);
            } else {
                m_configReader->skipCurrentElement();
            }
        } else if (m_configReader->name() == "PrivateTarget") {
            processPrivateTarget(targetName);
        } else if (m_configReader->name() == "samplingPeriod") {
            uint64_t samplingPeriod =
                m_configReader->readElementText().toULongLong(&ok, 10);
            if (ok) {
                GestureRecognizer::samplingPeriod = samplingPeriod;
            }
        } else if (m_configReader->name() == "pointerEmulationRate") {
            uint32_t pointerEmulationRate =
                m_configReader->readElementText().toULong(&ok, 10);
            if (ok) {
                GestureRecognizer::pointerEmulationRate = pointerEmulationRate;
            }
        } else if (m_configReader->name() == "pointerEmulationDistance") {
            float pointerEmulationDistance =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                GestureRecognizer::pointerEmulationDistance = pointerEmulationDistance;
            }
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processGlobalTarget()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "GlobalTarget") {
        return;
    }

    m_parsedTarget = GlobalTarget;
    while (m_configReader->readNextStartElement()) {
        if (!m_currentTarget) {
            m_currentTarget = new Target;
            Q_CHECK_PTR(m_currentTarget);
        }
        if (m_configReader->name() == "GestureRecognizers") {
            processGestureRecognizers();
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processPublicTarget(const QString& targetName)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "PublicTarget") {
        return;
    }

    QString name;
    bool match = false;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "name") {
            name = m_configReader->readElementText();
            if (name == targetName) {
                m_parsedTarget = PublicTarget;
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

void TargetFactory::processPrivateTarget(const QString& targetName)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "PrivateTarget") {
        return;
    }

    QString name;
    bool match = false;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "name") {
            name = m_configReader->readElementText();
            if (name == targetName) {
                m_parsedTarget = PrivateTarget;
                delete m_currentTarget;
                m_currentTarget = nullptr;
                m_currentTarget = new Target;
                Q_CHECK_PTR(m_currentTarget);
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
        } else if (m_configReader->name() == "Swipe") {
            processSwipe();
        } else {
            m_configReader->skipCurrentElement();
        }
    }
}

void TargetFactory::processGestureRecognizer(GestureRecognizer *gr)
{
    bool ok = false;
    if (m_configReader->name() == "recognitionThreshold") {
        float threshold =
            m_configReader->readElementText().toFloat(&ok);
        if (ok) {
            gr->setRecognitionThreshold(threshold);
        }
    } else if (m_configReader->name() == "abortList") {
        int id = 0;
        while (m_configReader->readNextStartElement()) {
            if (m_configReader->name() == "allowSimultaneousRecognition") {
                bool allowSimultaneousRecognition =
                    m_configReader->readElementText() == "true" ? true : false;
                gr->setAllowSimultaneousRecognition(allowSimultaneousRecognition);
            } else if (m_configReader->name() == "id") {
                    id = m_configReader->readElementText().toInt(&ok, 10);
                if (ok) {
                    m_abortTable[gr->id()].insert(id);
                }
            }
        }
    } else if (m_configReader->name() == "topMargin") {
        float topMargin = m_configReader->readElementText().toFloat(&ok);
        if (ok) {
            gr->setTopMargin(topMargin);
        }
    } else if (m_configReader->name() == "bottomMargin") {
        float bottomMargin = m_configReader->readElementText().toFloat(&ok);
        if (ok) {
            gr->setBottomMargin(bottomMargin);
        }
    } else if (m_configReader->name() == "leftMargin") {
        float leftMargin = m_configReader->readElementText().toFloat(&ok);
        if (ok) {
            gr->setLeftMargin(leftMargin);
        }
    } else if (m_configReader->name() == "rightMargin") {
        float rightMargin = m_configReader->readElementText().toFloat(&ok);
        if (ok) {
            gr->setRightMargin(rightMargin);
        }
    }
}

void TargetFactory::processLongPress()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "LongPress"
        || !m_configReader->attributes().hasAttribute("id")) {
        return;
    }

    bool ok = false;
    int id = 0;
    LongPressGestureRecognizer *gr = new LongPressGestureRecognizer;
    Q_CHECK_PTR(gr);
    id = m_configReader->attributes().value("id").toInt(&ok, 10);
    if (ok) {
        if (id <= 0) {
            return;
        }
        gr->setId(id);
    }

    while (m_configReader->readNextStartElement()) {
        processGestureRecognizer(gr);
        if (m_configReader->name() == "numTouchesRequired") {
            int numTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTouchesRequired(numTouchesRequired);
            }
        } else if (m_configReader->name() == "minPressDuration") {
            int minPressDuration =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMinPressDuration(minPressDuration);
            }
        } else if (m_configReader->name() == "XTestLongPressRightClick") {
            processXTestLongPressRightClick(gr);
        } else if (m_configReader->name() == "XTestLongPressMove") {
            processXTestLongPressMove(gr);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processPan()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Pan"
        || !m_configReader->attributes().hasAttribute("id")) {
        return;
    }

    bool ok = false;
    int id = 0;
    PanGestureRecognizer *gr = new PanGestureRecognizer;
    Q_CHECK_PTR(gr);
    id = m_configReader->attributes().value("id").toInt(&ok, 10);
    if (ok) {
        if (id <= 0) {
            return;
        }
        gr->setId(id);
    }

    while (m_configReader->readNextStartElement()) {
        processGestureRecognizer(gr);
        if (m_configReader->name() == "maxNumTouchesRequired") {
            int maxNumTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxNumTouchesRequired(maxNumTouchesRequired);
            }
        } else if (m_configReader->name() == "minNumTouchesRequired") {
            int minNumTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMinNumTouchesRequired(minNumTouchesRequired);
            }
        } else if (m_configReader->name() == "direction") {
            QString direction = m_configReader->readElementText();
            if (direction == "AnyDirection") {
                gr->setDirection(PanGestureRecognizer::AnyDirection);
            } else if (direction == "Horizontal") {
                gr->setDirection(PanGestureRecognizer::Horizontal);
            } else if (direction == "Vertical") {
                gr->setDirection(PanGestureRecognizer::Vertical);
            }
        } else if (m_configReader->name() == "XTestPanScroll") {
            processXTestPanScroll(gr);
        } else if (m_configReader->name() == "XTestPanMove") {
            processXTestPanMove(gr);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processTwoTouchPinch()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "TwoTouchPinch"
        || !m_configReader->attributes().hasAttribute("id")) {
        return;
    }

    bool ok = false;
    int id = 0;
    TwoTouchPinchGestureRecognizer * gr =
        new TwoTouchPinchGestureRecognizer;
    Q_CHECK_PTR(gr);
    id = m_configReader->attributes().value("id").toInt(&ok, 10);
    if (ok) {
        if (id <= 0) {
            return;
        }
        gr->setId(id);
    }

    while (m_configReader->readNextStartElement()) {
        processGestureRecognizer(gr);
        if (m_configReader->name() == "XTestTwoTouchPinchZoom") {
            processXTestTwoTouchPinchZoom(gr);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processTap()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Tap"
        || !m_configReader->attributes().hasAttribute("id")) {
        return;
    }

    bool ok = false;
    int id = 0;
    TapGestureRecognizer *gr = new TapGestureRecognizer;
    Q_CHECK_PTR(gr);
    id = m_configReader->attributes().value("id").toInt(&ok, 10);
    if (ok) {
        if (id <= 0) {
            return;
        }
        gr->setId(id);
    }

    while (m_configReader->readNextStartElement()) {
        processGestureRecognizer(gr);
        if (m_configReader->name() == "numTouchesRequired") {
            int numTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTouchesRequired(numTouchesRequired);
            }
        } else if(m_configReader->name() == "numTapsRequired") {
            int numTapsRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTapsRequired(numTapsRequired);
            }
        } else if(m_configReader->name() == "maxTapDelay") {
            int maxTapDelay =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxTapDelay(maxTapDelay);
            }
        } else if(m_configReader->name() == "maxTapDuration") {
            int maxTapDuration =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxTapDuration(maxTapDuration);
            }
        } else if(m_configReader->name() == "maxTapDistance") {
            float maxTapDistance =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxTapDistance(maxTapDistance);
            }
        } else if (m_configReader->name() == "XTestTapRightClick") {
            processXTestTapRightClick(gr);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processSwipe()
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "Swipe"
        || !m_configReader->attributes().hasAttribute("id")) {
        return;
    }

    bool ok = false;
    int id = 0;
    SwipeGestureRecognizer *gr = new SwipeGestureRecognizer;
    Q_CHECK_PTR(gr);
    id = m_configReader->attributes().value("id").toInt(&ok, 10);
    if (ok) {
        if (id <= 0) {
            return;
        }
        gr->setId(id);
    }

    while (m_configReader->readNextStartElement()) {
        processGestureRecognizer(gr);
        if (m_configReader->name() == "numTouchesRequired") {
            int numTouchesRequired =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setNumTouchesRequired(numTouchesRequired);
            }
        } else if(m_configReader->name() == "maxDuration") {
            int maxDuration =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                gr->setMaxDuration(maxDuration);
            }
        } else if(m_configReader->name() == "minDisplacement") {
            float minDisplacement =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMinDisplacement(minDisplacement);
            }
        } else if(m_configReader->name() == "maxAngle") {
            float maxAngle =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                gr->setMaxAngle(maxAngle);
            }
        } else if (m_configReader->name() == "direction") {
            SwipeGestureRecognizer::Direction direction
                = SwipeGestureRecognizer::AnyDirection;
            QString directionName =
                m_configReader->readElementText();
            if (directionName == "AnyDirection") {
                direction = SwipeGestureRecognizer::AnyDirection;
            } else if (directionName == "Left") {
                direction = SwipeGestureRecognizer::Left;
            } else if (directionName == "Right") {
                direction = SwipeGestureRecognizer::Right;
            } else if (directionName == "Up") {
                direction = SwipeGestureRecognizer::Up;
            } else if (directionName == "Down") {
                direction = SwipeGestureRecognizer::Down;
            } else if (directionName == "Vertical") {
                direction = SwipeGestureRecognizer::Vertical;
            } else if (directionName == "Horizontal") {
                direction = SwipeGestureRecognizer::Horizontal;
            } else if (directionName == "Orthogonal") {
                direction = SwipeGestureRecognizer::Orthogonal;
            }
            gr->setDirection(direction);
        } else if (m_configReader->name() == "XTestSwipeSwitch") {
            processXTestSwipeSwitch(gr);
        } else if (m_configReader->name() == "DBusSwipeKeyboard") {
            processDBusSwipeKeyboard(gr);
        }
    }
    m_currentTarget->addGestureRecognizer(gr);
}

void TargetFactory::processXTestSwipeSwitch(SwipeGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestSwipeSwitch") {
        return;
    }

    XTestSwipeSwitch *listener = new XTestSwipeSwitch;
    while(m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "SwitchRightShortcut") {
            listener->setSwitchRightShortcut(parseXTestShortcut());
        } else if (m_configReader->name() == "SwitchLeftShortcut") {
            listener->setSwitchLeftShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processDBusSwipeKeyboard(SwipeGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "DBusSwipeKeyboard") {
        return;
    }

    DBusSwipeKeyboard *listener = new DBusSwipeKeyboard;
    while(m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "showCommand") {
            listener->setShowCommand(m_configReader->readElementText());
        } else if (m_configReader->name() == "hideCommand") {
            listener->setHideCommand(m_configReader->readElementText());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestLongPressRightClick(LongPressGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestLongPressRightClick") {
        return;
    }

    XTestLongPressRightClick *listener = new XTestLongPressRightClick;
    while(m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "RightClickShortcut") {
            listener->setRightClickShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestTapRightClick(TapGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestTapRightClick") {
        return;
    }

    XTestTapRightClick *listener = new XTestTapRightClick;
    while(m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "RightClickShortcut") {
            listener->setRightClickShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestPanScroll(PanGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestPanScroll") {
        return;
    }

    XTestPanScroll *listener = new XTestPanScroll;
    bool ok = false;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "accumulator") {
            int accumulator =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                listener->setAccumulator(accumulator);
            }
        } else if (m_configReader->name() == "maxVelocity") {
            float maxVelocity =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                listener->setMaxVelocity(maxVelocity);
            }
        } else if (m_configReader->name() == "minVelocity") {
            float minVelocity =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                listener->setMinVelocity(minVelocity);
            }
        } else if (m_configReader->name() == "ScrollLeftShortcut") {
            listener->setScrollLeftShortcut(parseXTestShortcut());
        } else if (m_configReader->name() == "ScrollRightShortcut") {
            listener->setScrollRightShortcut(parseXTestShortcut());
        } else if (m_configReader->name() == "ScrollUpShortcut") {
            listener->setScrollUpShortcut(parseXTestShortcut());
        } else if (m_configReader->name() == "ScrollDownShortcut") {
            listener->setScrollDownShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestPanMove(PanGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestPanMove") {
        return;
    }

    XTestPanMove *listener = new XTestPanMove;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "MoveShortcut") {
            listener->setMoveShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestLongPressMove(LongPressGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestLongPressMove") {
        return;
    }

    XTestLongPressMove *listener = new XTestLongPressMove;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "MoveShortcut") {
            listener->setMoveShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

void TargetFactory::processXTestTwoTouchPinchZoom(
                            TwoTouchPinchGestureRecognizer *gr)
{
    if (!m_configReader->isStartElement()
        || m_configReader->name() != "XTestTwoTouchPinchZoom") {
        return;
    }

    bool ok = false;
    XTestTwoTouchPinchZoom *listener = new XTestTwoTouchPinchZoom;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "accumulator") {
            int accumulator =
                m_configReader->readElementText().toInt(&ok, 10);
            if (ok) {
                listener->setAccumulator(accumulator);
            }
        } else if (m_configReader->name() == "maxScale") {
            float maxScale =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                listener->setMaxScale(maxScale);
            }
        } else if (m_configReader->name() == "minScale") {
            float minScale =
                m_configReader->readElementText().toFloat(&ok);
            if (ok) {
                listener->setMinScale(minScale);
            }
        } else if (m_configReader->name() == "ZoomInShortcut") {
            listener->setZoomInShortcut(parseXTestShortcut());
        } else if (m_configReader->name() == "ZoomOutShortcut") {
            listener->setZoomOutShortcut(parseXTestShortcut());
        }
    }
    listener->setGestureRecognizer(gr);
}

XTestShortcut* TargetFactory::parseXTestShortcut()
{
    if (!m_configReader->isStartElement()) {
        return nullptr;
    }

    XTestShortcut *shortcut = new XTestShortcut;
    QString value;
    QString modifier;
    while (m_configReader->readNextStartElement()) {
        if (m_configReader->name() == "button") {
            for (int i = 0; i < m_configReader->attributes().size(); ++i) {
                modifier = m_configReader->attributes().at(i).value().toString();
                shortcut->addModifier(modifier);
            }
            value = m_configReader->readElementText();
            shortcut->setValue(value, XTestShortcut::Button);
        } else if (m_configReader->name() == "key") {
            for (int i = 0; i < m_configReader->attributes().size(); ++i) {
                modifier = m_configReader->attributes().at(i).value().toString();
                shortcut->addModifier(modifier);
            }
            value = m_configReader->readElementText();
            shortcut->setValue(value, XTestShortcut::Key);
        }
    }
    return shortcut;
}
