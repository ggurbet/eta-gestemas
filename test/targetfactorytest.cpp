#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "targetfactory.h"
#include "target.h"
#include "longpressgesturerecognizer.h"
#include "pangesturerecognizer.h"
#include "twotouchpinchgesturerecognizer.h"
#include "tapgesturerecognizer.h"
#include "swipegesturerecognizer.h"

#include "xtestsimple.h"
#include "xtestscroll.h"
#include "xtestzoom.h"
#include "xtestmove.h"
#include "dbusvirtualkeyboard.h"

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
    XTestSimple *l1 = new XTestSimple;
    l1->setGestureRecognizer(gr1);
    gr1->setGestureListener(l1);
    expectedTargetChromium.addGestureRecognizer(gr1);
    PanGestureRecognizer *gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    XTestScroll *l2 = new XTestScroll;
    l2->setMinEffectiveVelocity(2.0f);
    l2->setGestureRecognizer(gr2);
    gr2->setGestureListener(l2);
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
    XTestScroll *l4 = new XTestScroll;
    l4->setMinEffectiveVelocity(2.0f);
    l4->setGestureRecognizer(gr4);
    gr4->setGestureListener(l4);
    expectedTargetChromium.addGestureRecognizer(gr4);
    TwoTouchPinchGestureRecognizer *gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(false);
    XTestZoom *l5 = new XTestZoom;
    l5->setGestureRecognizer(gr5);
    gr5->setGestureListener(l5);
    expectedTargetChromium.addGestureRecognizer(gr5);
    SwipeGestureRecognizer *gr6 = new SwipeGestureRecognizer;
    gr6->setRecognitionThreshold(1.0f);
    gr6->setNumTouchesRequired(2);
    gr6->setMaxDuration(400);
    gr6->setMinDisplacement(0.002f);
    gr6->setMaxAngle(40.0f);
    gr6->setDirection(SwipeGestureRecognizer::AnyDirection);
    gr6->setAllowSimultaneousRecognition(false);
    expectedTargetChromium.addGestureRecognizer(gr6);

    expectedTargetFirefox.setTargetId(201);
    expectedTargetFirefox.setTargetName("Firefox");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    l1 = new XTestSimple;
    l1->setGestureRecognizer(gr1);
    gr1->setGestureListener(l1);
    expectedTargetFirefox.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    l2 = new XTestScroll;
    l2->setMinEffectiveVelocity(2.0f);
    l2->setGestureRecognizer(gr2);
    gr2->setGestureListener(l2);
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
    l4 = new XTestScroll;
    l4->setMinEffectiveVelocity(2.0f);
    l4->setGestureRecognizer(gr4);
    gr4->setGestureListener(l4);
    expectedTargetFirefox.addGestureRecognizer(gr4);
    gr5 = new TwoTouchPinchGestureRecognizer;
    gr5->setRecognitionThreshold(1.0f);
    gr5->setAllowSimultaneousRecognition(true);
    l5 = new XTestZoom;
    l5->setGestureRecognizer(gr5);
    gr5->setGestureListener(l5);
    expectedTargetFirefox.addGestureRecognizer(gr5);
    gr6 = new SwipeGestureRecognizer;
    gr6->setRecognitionThreshold(1.0f);
    gr6->setNumTouchesRequired(2);
    gr6->setMaxDuration(400);
    gr6->setMinDisplacement(0.002f);
    gr6->setMaxAngle(40.0f);
    gr6->setDirection(SwipeGestureRecognizer::AnyDirection);
    gr6->setAllowSimultaneousRecognition(false);
    expectedTargetFirefox.addGestureRecognizer(gr6);

    expectedTargetOther.setTargetId(202);
    expectedTargetOther.setTargetName("Other");
    gr1 = new LongPressGestureRecognizer;
    gr1->setNumTouchesRequired(1);
    gr1->setMinPressDuration(950);
    gr1->setRecognitionThreshold(1.0f);
    gr1->setAllowSimultaneousRecognition(false);
    l1 = new XTestSimple;
    l1->setGestureRecognizer(gr1);
    gr1->setGestureListener(l1);
    expectedTargetOther.addGestureRecognizer(gr1);
    gr2 = new PanGestureRecognizer;
    gr2->setMaxNumTouchesRequired(1);
    gr2->setMinNumTouchesRequired(1);
    gr2->setRecognitionThreshold(1.0f);
    gr2->setAllowSimultaneousRecognition(false);
    l2 = new XTestScroll;
    l2->setMinEffectiveVelocity(2.0f);
    l2->setGestureRecognizer(gr2);
    gr2->setGestureListener(l2);
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
