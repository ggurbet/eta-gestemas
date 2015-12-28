#ifndef TARGETFACTORY_H
#define TARGETFACTORY_H


class QXmlStreamReader;
class QFile;
class QString;

class GestureListener;
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

    void setGestureListener(GestureListener *listener);
    GestureListener* gestureListener() const;

    TargetFactory(const TargetFactory&) = delete;
    TargetFactory& operator=(const TargetFactory&) = delete;
private:
    void processTargets(const QString& targetName);
    void processTarget(const QString& targetName);
    void processGestureRecognizers();
    void processLongPress();
    void processPan();
    void processTwoTouchPinch();
    void processTap();

    QXmlStreamReader *m_configReader;
    QFile *m_configFile;
    GestureListener *m_listener;
    Target *m_currentTarget;
};

#endif /* TARGETFACTORY_H */
