#ifndef TARGETFACTORY_H
#define TARGETFACTORY_H

#include <QtCore/QHash>
#include <QtCore/QSet>

class QXmlStreamReader;
class QFile;
class QString;

class Target;

class GestureRecognizer;
class LongPressGestureRecognizer;
class PanGestureRecognizer;
class TwoTouchPinchGestureRecognizer;
class TapGestureRecognizer;
class SwipeGestureRecognizer;

class XTestOnBegan;
class XTestOnEnded;
class XTestOnRecognized;
class XTestMove;
class XTestZoom;
class XTestScroll;
class DBusVirtualKeyboard;

class XTestShortcut;

class TargetFactory
{
public:
    TargetFactory();
    explicit TargetFactory(const QString& configFileName);
    ~TargetFactory();

    Target* create(unsigned long targetId, const QString& targetName);

    void setConfigurationFileName(const QString& configFileName);
    QString configFileName() const;

    TargetFactory(const TargetFactory&) = delete;
    TargetFactory& operator=(const TargetFactory&) = delete;
private:
    enum ParsedTargetType {
        UndefinedTarget,
        GlobalTarget,
        PublicTarget,
        PrivateTarget
    };
    void processAll(const QString& targetName);
    void processGlobalTarget();
    void processPublicTarget(const QString& targetName);
    void processPrivateTarget(const QString& targetName);
    void processGestureRecognizers();
    void processGestureRecognizer(GestureRecognizer *gr);

    void processLongPress();
    void processPan();
    void processTwoTouchPinch();
    void processTap();
    void processSwipe();

    XTestOnBegan* parseXTestOnBegan();
    XTestOnEnded* parseXTestOnEnded();
    XTestOnRecognized* parseXTestOnRecognized();
    XTestScroll* parseXTestScroll();
    XTestMove* parseXTestMove();
    XTestZoom* parseXTestZoom();
    DBusVirtualKeyboard* parseDBusVirtualKeyboard();

    XTestShortcut* parseXTestShortcut();

    QXmlStreamReader *m_configReader;
    QFile *m_configFile;
    Target *m_currentTarget;
    QHash<int, QSet<int> > m_abortTable;
    ParsedTargetType m_parsedTarget;
};

#endif /* TARGETFACTORY_H */
