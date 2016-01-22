#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"
#include "swipegesturerecognizer.h"

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
    Target expectedTargetEog;
    Target *parsedTargetChromium;
    Target *parsedTargetFirefox;
    Target *parsedTargetOther;
    Target *parsedTargetOkular;
    Target *parsedTargetEog;
};

void TargetFactoryTester::initTestCase()
{
    // Expected target after parsing process
    expectedTargetChromium.setTargetId(200);
    expectedTargetChromium.setTargetName("Chromium");
    LongPressGestureRecognizer *gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr1);
    PanGestureRecognizer *gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr2);
    TapGestureRecognizer *gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr3);
    PanGestureRecognizer *gr4 = new PanGestureRecognizer;
    gr4->setMaxNumTouchesRequired(2);
    gr4->setMinNumTouchesRequired(2);
    gr4->setRecognitionThreshold(1.0f);
    gr4->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr4);
    TwoTouchPinchGestureRecognizer *gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr5);
    SwipeGestureRecognizer *gr6 = new SwipeGestureRecognizer;
    gr6->setRecognitionThreshold(1.0f);
    gr6->setNumTouchesRequired(2);
    gr6->setMaxDuration(400);
    gr6->setMinDisplacement(0.002f);
    gr6->setMaxAngle(40.0f);
    gr6->setDirection(SwipeGestureRecognizer::NoDirection);
    gr6->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr6);

    expectedTargetFirefox.setTargetId(201);
    expectedTargetFirefox.setTargetName("Firefox");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr2);
    gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr3);
    gr4 = new PanGestureRecognizer;
    gr4->setMaxNumTouchesRequired(2);
    gr4->setMinNumTouchesRequired(2);
    gr4->setRecognitionThreshold(1.0f);
    gr4->setAllowSimultaneousRecognition(true);
    expectedTargetFirefox.addGestureRecognizer(gr4);
    gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(true);
    expectedTargetFirefox.addGestureRecognizer(gr5);
    gr6 = new SwipeGestureRecognizer;
    gr6->setRecognitionThreshold(1.0f);
    gr6->setNumTouchesRequired(2);
    gr6->setMaxDuration(400);
    gr6->setMinDisplacement(0.002f);
    gr6->setMaxAngle(40.0f);
    gr6->setDirection(SwipeGestureRecognizer::NoDirection);
    gr6->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr6);

    expectedTargetOther.setTargetId(202);
    expectedTargetOther.setTargetName("Other");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetOther.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    expectedTargetOther.addGestureRecognizer(gr2);
    gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    expectedTargetOther.addGestureRecognizer(gr3);

    expectedTargetEog.setTargetId(204);
    expectedTargetEog.setTargetName("Eog");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    expectedTargetEog.addGestureRecognizer(gr1);

    // Parsed targets
    targetFactory.setConfigurationFileName("config.xml");
    parsedTargetChromium = targetFactory.create(200, "Chromium");
    parsedTargetFirefox = targetFactory.create(201, "Firefox");
    parsedTargetOther = targetFactory.create(202, "Other");
    parsedTargetOkular = targetFactory.create(203, "Okular");
    parsedTargetEog = targetFactory.create(204, "Eog");
}

void TargetFactoryTester::cleanupTestCase()
{
    delete parsedTargetChromium;
    delete parsedTargetFirefox;
    delete parsedTargetOther;
    delete parsedTargetOkular;
    delete parsedTargetEog;
}

void TargetFactoryTester::testTargetFactory()
{
    QVERIFY(parsedTargetChromium->isEqual(expectedTargetChromium));
    QVERIFY(parsedTargetFirefox->isEqual(expectedTargetFirefox));
    QVERIFY(parsedTargetOther->isEqual(expectedTargetOther));
    QVERIFY(parsedTargetOkular == nullptr);
    QVERIFY(parsedTargetEog->isEqual(expectedTargetEog));
    QVERIFY(!parsedTargetOther->isEqual(expectedTargetChromium));
    QVERIFY(!parsedTargetOther->isEqual(expectedTargetFirefox));
    QVERIFY(!parsedTargetChromium->isEqual(expectedTargetFirefox));
}

QTEST_MAIN(TargetFactoryTester)
#include "targetfactorytest.moc"
