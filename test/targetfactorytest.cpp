#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"
#include "rightclick.h"
#include "leftclick.h"
#include "drag.h"
#include "scroll.h"
#include "zoom.h"

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
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    RightClick *listener1 = new RightClick;
    listener1->setGestureRecognizer(gr1);
    expectedTargetChromium.addGestureRecognizer(gr1);
    PanGestureRecognizer *gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    Drag *listener2 = new Drag;
    listener2->setGestureRecognizer(gr2);
    expectedTargetChromium.addGestureRecognizer(gr2);
    TapGestureRecognizer *gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    LeftClick *listener3 = new LeftClick;
    listener3->setGestureRecognizer(gr3);
    expectedTargetChromium.addGestureRecognizer(gr3);
    PanGestureRecognizer *gr4 = new PanGestureRecognizer;
    gr4->setMaxNumTouchesRequired(2);
    gr4->setMinNumTouchesRequired(2);
    gr4->setRecognitionThreshold(1.0f);
    gr4->setAllowSimultaneousRecognition(false);
    Scroll *listener4 = new Scroll;
    listener4->setAccumulator(10);
    listener4->setMaxVelocity(20.0f);
    listener4->setMinVelocity(1.0f);
    listener4->setGestureRecognizer(gr4);
    expectedTargetChromium.addGestureRecognizer(gr4);
    TwoTouchPinchGestureRecognizer *gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(false);
    Zoom *listener5 = new Zoom;
    listener5->setAccumulator(10);
    listener5->setMaxScale(3.0f);
    listener5->setMinScale(1.2f);
    listener5->setGestureRecognizer(gr5);
    expectedTargetChromium.addGestureRecognizer(gr5);

    expectedTargetFirefox.setTargetId(201);
    expectedTargetFirefox.setTargetName("Firefox");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    listener1 = new RightClick;
    listener1->setGestureRecognizer(gr1);
    expectedTargetFirefox.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    listener2 = new Drag;
    listener2->setGestureRecognizer(gr2);
    expectedTargetFirefox.addGestureRecognizer(gr2);
    gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    listener3 = new LeftClick;
    listener3->setGestureRecognizer(gr3);
    expectedTargetFirefox.addGestureRecognizer(gr3);
    gr4 = new PanGestureRecognizer;
    gr4->setMaxNumTouchesRequired(2);
    gr4->setMinNumTouchesRequired(2);
    gr4->setRecognitionThreshold(1.0f);
    gr4->setAllowSimultaneousRecognition(true);
    listener4 = new Scroll;
    listener4->setAccumulator(15);
    listener4->setMaxVelocity(20.0f);
    listener4->setMinVelocity(1.0f);
    listener4->setGestureRecognizer(gr4);
    expectedTargetFirefox.addGestureRecognizer(gr4);
    gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(true);
    listener5 = new Zoom;
    listener5->setAccumulator(10);
    listener5->setMaxScale(3.0f);
    listener5->setMinScale(1.2f);
    listener5->setGestureRecognizer(gr5);
    expectedTargetFirefox.addGestureRecognizer(gr5);

    expectedTargetOther.setTargetId(202);
    expectedTargetOther.setTargetName("Other");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    listener1 = new RightClick;
    listener1->setGestureRecognizer(gr1);
    expectedTargetOther.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    listener2 = new Drag;
    listener2->setGestureRecognizer(gr2);
    expectedTargetOther.addGestureRecognizer(gr2);
    gr3 = new TapGestureRecognizer;
    gr3->setNumTouchesRequired(1);
    gr3->setNumTapsRequired(1);
    gr3->setMaxTapDelay(400);
    gr3->setMaxTapDuration(1000);
    gr3->setMaxTapDistance(100.0f);
    gr3->setRecognitionThreshold(1.0f);
    gr3->setAllowSimultaneousRecognition(false);
    listener3 = new LeftClick;
    listener3->setGestureRecognizer(gr3);
    expectedTargetOther.addGestureRecognizer(gr3);

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
