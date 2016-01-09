#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"

class TargetFactoryTester : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testTargetFactory();
private:
    TargetFactory targetFactory;
    Target expectedTargetChromium;
    Target expectedTargetFirefox;
    Target expectedTargetOther;
    Target *parsedTargetChromium;
    Target *parsedTargetFirefox;
    Target *parsedTargetOther;
};

void TargetFactoryTester::initTestCase()
{
    // Expected target after parsing process
    expectedTargetChromium.setTargetId(200);
    expectedTargetChromium.setTargetName("Chromium");
    LongPressGestureRecognizer *gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setMaxAllowableDrift(0.001f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr1);
    PanGestureRecognizer *gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(2);
    gr2->setMinNumTouchesRequired(2);
    gr2->setMaxVelocity(20.0f);
    gr2->setMinVelocity(0.1f);
    gr2->setMaxAllowableDrift(0.001f);
    gr2->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr2);
    TwoTouchPinchGestureRecognizer *gr3 = new TwoTouchPinchGestureRecognizer;
    gr3->setMaxScale(3.0f);
    gr3->setMinScale(1.2);
    gr3->setMaxAllowableDrift(0.001f);
    gr3->setAllowSimultaneousRecognition(false);
    gr3->setAccumulator(10);
    expectedTargetChromium.addGestureRecognizer(gr3);
    TapGestureRecognizer *gr4 = new TapGestureRecognizer;
    gr4->setNumTouchesRequired(2);
    gr4->setNumTapsRequired(1);
    gr4->setMaxTapDelay(400);
    gr4->setMaxTapDuration(1000);
    gr4->setMaxTapDistance(100.0f);
    gr4->setMaxAllowableDrift(0.001f);
    gr4->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr4);

    expectedTargetFirefox.setTargetId(201);
    expectedTargetFirefox.setTargetName("Firefox");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setMaxAllowableDrift(0.001f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(3);
    gr2->setMinNumTouchesRequired(3);
    gr2->setMaxVelocity(20.2f);
    gr2->setMinVelocity(0.2f);
    gr2->setMaxAllowableDrift(0.001f);
    expectedTargetFirefox.addGestureRecognizer(gr2);
    gr3 = new TwoTouchPinchGestureRecognizer;
    gr3->setMaxScale(4.0f);
    gr3->setMinScale(1.4);
    gr3->setMaxAllowableDrift(0.001f);
    gr3->setAllowSimultaneousRecognition(true);
    gr3->setAccumulator(11);
    expectedTargetFirefox.addGestureRecognizer(gr3);
    gr4 = new TapGestureRecognizer;
    gr4->setNumTouchesRequired(3);
    gr4->setNumTapsRequired(1);
    gr4->setMaxTapDelay(400);
    gr4->setMaxTapDuration(1000);
    gr4->setMaxTapDistance(100.0f);
    gr4->setMaxAllowableDrift(0.0001f);
    gr4->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr4);

    expectedTargetOther.setTargetId(202);
    expectedTargetOther.setTargetName("Other");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setMaxAllowableDrift(0.001f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetOther.addGestureRecognizer(gr1);

    // Parsed targets
    targetFactory.setConfigurationFileName("config.xml");
    parsedTargetChromium = targetFactory.create(200, "Chromium");
    parsedTargetFirefox = targetFactory.create(201, "Firefox");
    parsedTargetOther = targetFactory.create(202, "Other");
}

void TargetFactoryTester::cleanupTestCase()
{
    delete parsedTargetChromium;
    delete parsedTargetFirefox;
    delete parsedTargetOther;
}

void TargetFactoryTester::testTargetFactory()
{
    QVERIFY(parsedTargetChromium->isEqual(expectedTargetChromium));
    QVERIFY(parsedTargetFirefox->isEqual(expectedTargetFirefox));
    QVERIFY(parsedTargetOther->isEqual(expectedTargetOther));
    QVERIFY(!parsedTargetOther->isEqual(expectedTargetChromium));
    QVERIFY(!parsedTargetOther->isEqual(expectedTargetFirefox));
    QVERIFY(!parsedTargetChromium->isEqual(expectedTargetFirefox));
}

QTEST_MAIN(TargetFactoryTester)
#include "targetfactorytest.moc"
