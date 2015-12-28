#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "state.h"

Q_DECLARE_METATYPE(State)

class StateTester : public QObject
{
    Q_OBJECT

private slots:
    void testTransition_data();
    void testTransition();
};

void StateTester::testTransition_data()
{
    QTest::addColumn<State>("from");
    QTest::addColumn<State>("to");
    QTest::addColumn<bool>("valid");

    QTest::newRow("Possible -> Recognized") << State::Possible << State::Recognized << true;
    QTest::newRow("Possible -> Failed") << State::Possible << State::Failed << true;
    QTest::newRow("Possible -> Began") << State::Possible << State::Began << true;

    QTest::newRow("Recognized -> Possible") << State::Recognized << State::Possible << true;

    QTest::newRow("Began -> Changed") << State::Began << State::Changed << true;
    QTest::newRow("Began -> Ended") << State::Began << State::Ended << true;
    QTest::newRow("Began -> Canceled") << State::Began << State::Canceled << true;

    QTest::newRow("Changed -> Changed") << State::Changed << State::Changed << true;
    QTest::newRow("Changed -> Ended") << State::Changed << State::Ended << true;
    QTest::newRow("Changed -> Canceled") << State::Changed << State::Canceled << true;

    QTest::newRow("Ended -> Possible") << State::Ended << State::Possible << true;

    QTest::newRow("Failed -> Possible") << State::Failed << State::Possible << true;

    QTest::newRow("Canceled -> Possible") << State::Canceled << State::Possible << true;

    QTest::newRow("Possible -> Canceled") << State::Possible << State::Canceled << false;
    QTest::newRow("Possible -> Changed") << State::Possible << State::Changed << false;
    QTest::newRow("Failed -> Began") << State::Failed << State::Began << false;
    QTest::newRow("Changed -> Possible") << State::Changed << State::Possible << false;
    QTest::newRow("Changed -> Began") << State::Changed << State::Began << false;

}

void StateTester::testTransition()
{
    QFETCH(State, from);
    QFETCH(State, to);
    QFETCH(bool, valid);

    QCOMPARE(from.canTransitionTo(to), valid);
}

QTEST_MAIN(StateTester)
#include "statetest.moc"
