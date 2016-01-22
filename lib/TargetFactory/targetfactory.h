#ifndef TARGETFACTORY_H
#define TARGETFACTORY_H

#include <QtCore/QHash>
#include <QtCore/QSet>

class QXmlStreamReader;
class QFile;
class QString;

class GestureRecognizer;
class LongPressGestureRecognizer;
class PanGestureRecognizer;
class TwoTouchPinchGestureRecognizer;
class TapGestureRecognizer;
class Target;

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

    void processRightClick(LongPressGestureRecognizer *gr);
    void processDrag(PanGestureRecognizer *gr);
    void processScroll(PanGestureRecognizer *gr);
    void processMove(PanGestureRecognizer *gr);
    void processLeftClick(TapGestureRecognizer *gr);
    void processZoom(TwoTouchPinchGestureRecognizer *gr);

    QXmlStreamReader *m_configReader;
    QFile *m_configFile;
    Target *m_currentTarget;
    QHash<int, QSet<int> > m_abortTable;
    ParsedTargetType m_parsedTarget;
};

#endif /* TARGETFACTORY_H */
