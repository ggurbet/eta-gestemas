#include "xlibwindowmanageradapter.h"
#include <QtTest/QtTest>
#include <QtWidgets/QWidget>

class XLibWindowManagerAdapterTester :public QObject,
                                      public WindowManagerAdapterListenerInterface
{
    Q_OBJECT
public:
    virtual void onWindowCreated(unsigned long targetId,
                                 const QString& targetName, bool *grabTouches);
    virtual void onWindowDestroyed(unsigned long targetId);
    virtual void onTouchEvent(void *data);

private slots:
    void initTestCase();
    void testOnWindowCreated_data();
    void testOnWindowCreated();
    void testOnWindowDestroyed_data();
    void testOnWindowDestroyed();
    void cleanupTestCase();

private:
    const static int NUMBER_OF_WINDOWS = 10;
    QList<QWidget *> createdWidgets;
    QList<unsigned long> createdWindowIds;
    QList<unsigned long> capturedCreatedWindowIds;
    QList<QString> createdWindowNames;
    QList<QString> capturedCreatedWindowNames;
    QList<unsigned long> destroyedWindowIds;
    QList<unsigned long> capturedDestroyedWindowIds;
    XLibWindowManagerAdapter *xlibwma;

};

void XLibWindowManagerAdapterTester::initTestCase()
{
    xlibwma = new XLibWindowManagerAdapter(this);
    xlibwma->setListener(this);
    xlibwma->dispatchEvents();
    QWidget *w;
    capturedCreatedWindowIds.clear();
    capturedCreatedWindowNames.clear();
    for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        w = new QWidget;
        w->resize(320, 240);
        w->show();
        QTest::qWait(500);
        createdWindowIds.append(w->winId());
        createdWindowNames.append("xlibwmatest");
        createdWidgets.append(w);
    }

    while(!createdWidgets.isEmpty()) {
        w = createdWidgets.takeFirst();
        destroyedWindowIds.append(w->winId());
        w->close();
        QTest::qWait(500);
        w->deleteLater();
    }
}

void XLibWindowManagerAdapterTester::testOnWindowCreated_data()
{
    QTest::addColumn<unsigned long>("createdId");
    QTest::addColumn<unsigned long>("capturedCreatedId");
    QTest::addColumn<QString>("createdName");
    QTest::addColumn<QString>("capturedCreatedName");

    for(int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        QTest::newRow(QString::number(i + 1, 10).toLatin1().constData())
            << createdWindowIds.at(i) << capturedCreatedWindowIds.at(i)
            << createdWindowNames.at(i) << capturedCreatedWindowNames.at(i);
    }
}

void XLibWindowManagerAdapterTester::testOnWindowCreated()
{
    QFETCH(unsigned long, createdId);
    QFETCH(unsigned long, capturedCreatedId);
    QCOMPARE(createdId, capturedCreatedId);

    QFETCH(QString, createdName);
    QFETCH(QString, capturedCreatedName);
    QCOMPARE(createdName, capturedCreatedName);
}

void XLibWindowManagerAdapterTester::testOnWindowDestroyed_data()
{
    QTest::addColumn<unsigned long>("destroyedId");
    QTest::addColumn<unsigned long>("capturedDestroyedId");

    for(int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        QTest::newRow(QString::number(i + 1, 10).toLatin1().constData())
            << destroyedWindowIds.at(i) << capturedDestroyedWindowIds.at(i);
    }
}

void XLibWindowManagerAdapterTester::testOnWindowDestroyed()
{
    QFETCH(unsigned long, destroyedId);
    QFETCH(unsigned long, capturedDestroyedId);
    QCOMPARE(destroyedId, capturedDestroyedId);
}

void XLibWindowManagerAdapterTester::cleanupTestCase()
{
}


void XLibWindowManagerAdapterTester::onWindowCreated(unsigned long targetId,
                                                     const QString& targetName, bool *grabTouches)
{
    capturedCreatedWindowIds.append(targetId);
    capturedCreatedWindowNames.append(targetName);
    if (grabTouches) {
        *grabTouches = true;
    }
}

void XLibWindowManagerAdapterTester::onWindowDestroyed(unsigned long targetId)
{
    capturedDestroyedWindowIds.append(targetId);
}

void XLibWindowManagerAdapterTester::onTouchEvent(void *data)
{
    (void)data;
    //TODO: find a way to test touch events automatically.
    // Consider libevdev, uinput...
}

QTEST_MAIN(XLibWindowManagerAdapterTester)
#include "xlibwmatest.moc"
