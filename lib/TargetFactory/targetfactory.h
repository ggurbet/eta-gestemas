#ifndef TARGETFACTORY_H
#define TARGETFACTORY_H


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
    void processAll(const QString& targetName);
    void processTarget(const QString& targetName);
    void processGestureRecognizers();
    void processGestureRecognizer(GestureRecognizer *gr);
    void processLongPress();
    void processPan();
    void processTwoTouchPinch();
    void processTap();

    void processRightClick(LongPressGestureRecognizer *gr);
    void processDrag(PanGestureRecognizer *gr);
    void processScroll(PanGestureRecognizer *gr);
    void processLeftClick(TapGestureRecognizer *gr);
    void processZoom(TwoTouchPinchGestureRecognizer *gr);

    QXmlStreamReader *m_configReader;
    QFile *m_configFile;
    Target *m_currentTarget;
};

#endif /* TARGETFACTORY_H */
