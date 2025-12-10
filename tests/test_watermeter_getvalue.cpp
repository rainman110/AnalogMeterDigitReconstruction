#include "ClassAnalogDigitalMeter.h"

#include <unity.h>
#include <unity_internals.h>


void setUp(void) {}
void tearDown(void) {}

void flowPP_case01(void)
{
    ClassAnalogDigitalMeter meter(2, 2);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 16.98, meter.getValue({1.2, 6.7, 9.5, 8.4}).value);
}

void flowPP_case02(void)
{
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 376529.6, meter.getValue({3.0, 7.0, 6.0, 5.0, 2.5, 9.6, 6.4}).value);
}

void flowPP_case03(void)
{
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 167734.6, meter.getValue({1.1, 6.0, 7.0, 7.0, 3.0, 4.6, 6.2}).value);
}

void flowPP_case04(void)
{
    ClassAnalogDigitalMeter meter(2, 5);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 58.96889, meter.getValue({5.0, 8.6, 9.8, 6.7, 8.9, 8.6, 9.8}).value);
}

void flowPP_case05(void)
{
    ClassAnalogDigitalMeter meter(6, 1);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 377083.9, meter.getValue({2.9, 7.0, 6.8, 9.9, 8.0, 3.9, 9.7}).value);
}

void flowPP_case06(void)
{
    ClassAnalogDigitalMeter meter(3, 3);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 199.748, meter.getValue({1.3, 9.0, 9.0, 7.1, 4.8, 8.3}).value);
}

void flowPP_case07(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/942#issuecomment-1226966346
    ClassAnalogDigitalMeter meter(6, 0);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33338.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 3.0, 8.9}).value);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33339.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 3.0, 9.0}).value);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33339.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 3.4, 9.4}).value);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33339.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 3.5, 9.5}).value);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33339.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 3.9, 9.9}).value);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 33340.f, meter.getValue({0.0, 2.9, 3.0, 2.9, 4.0, 0.0}).value);
}

void flowPP_case08(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/942
    ClassAnalogDigitalMeter meter(8, 0);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 704178.f,
                             meter.getValue({0.0, 9.9, 6.8, 9.9, 3.7, 0.8, 6.9, 8.0}).value);
}

void flowPP_case09(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/942#issuecomment-1228343319
    ClassAnalogDigitalMeter meter(8, 0);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 7153692.f,
                             meter.getValue({9.9, 6.8, 1.1, 4.7, 2.7, 6.0, 9.0, 2.8}).value);
}

void flowPP_case10(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/942#issuecomment-1228343319
    ClassAnalogDigitalMeter meter(3, 4);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 194.9089,
                             meter.getValue({1.0, 9.0, 4.3, 8.9, 0.7, 8.9, 9.4}).value);
}

void flowPP_case11(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/921#issuecomment-1229552041
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 377988.4,
                             meter.getValue({2.9, 7.0, 7.0, 9.1, 8.1, 8.5, 4.1}).value);
}

void flowPP_case12(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/921#issuecomment-1233149877
    ClassAnalogDigitalMeter meter(4, 4);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 78.9999,
                             meter.getValue({0.0, 0.0, 7.0, 8.9, 0.1, 0.1, 0.1, 9.6}).value);
}

void flowPP_case13(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/921#issuecomment-1236119370
    ClassAnalogDigitalMeter meter(3, 4);
    // we model the misbehaviour of the meter with early transition
    meter.setAnalogDigitalTransitionEnd(9);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 395.8632,
                             meter.getValue({3.1, 9.1, 5.7, 8.8, 6.1, 3.0, 2.0}).value);
}

void flowPP_case14(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/921#issuecomment-1236119370
    ClassAnalogDigitalMeter meter(3, 4);
    // we model the misbehaviour of the meter with a constantly rolling last digit
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 395.8632,
                             meter.getValue({3.1, 9.1, 5.7, 8.8, 6.1, 3.0, 2.0}).value);
}

void flowPP_case15(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/discussions/950#discussion-4338615
    ClassAnalogDigitalMeter meter(3, 3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 199.748,
                             meter.getValue({1.0, 9.0, 9.0, 7.1, 4.8, 8.3}).value);
}

void flowPP_case16(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/921#issuecomment-1242730397
    ClassAnalogDigitalMeter meter(8, 0);
    meter.setNDecimalPlaces(3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 32289.419,
                             meter.getValue({3.0, 2.0, 2.0, 8.0, 9.0, 4.0, 1.7, 9.8}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 32289.4198,
                             meter.getValue({3.0, 2.0, 2.0, 8.0, 9.0, 4.0, 1.7, 9.8}, true).value);
}

void flowPP_case17(void)
{
    // not documented as issue
    ClassAnalogDigitalMeter meter(4, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 83.9940,
                             meter.getValue({0.0, 0.0, 7.9, 3.8, 0.0, 9.4, 4.1, 0.1}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 83.99401,
                             meter.getValue({0.0, 0.0, 7.9, 3.8, 0.0, 9.4, 4.1, 0.1}, true).value);
}

void flowPP_case18(void)
{
    // not documented as issue
    ClassAnalogDigitalMeter meter(8, 0);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 123235.f,
                             meter.getValue({0.0, 0.0, 1.0, 2.0, 2.8, 1.9, 2.8, 5.6}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 123235.6,
                             meter.getValue({0.0, 0.0, 1.0, 2.0, 2.8, 1.9, 2.8, 5.6}, true).value);
}

void flowPP_case19(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/discussions/950#discussioncomment-3661982
    ClassAnalogDigitalMeter meter(7, 0);
    meter.setNDecimalPlaces(3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 3249.469,
                             meter.getValue({3.0, 2.0, 4.1, 9.0, 4.0, 6.3, 9.2}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 3249.4692,
                             meter.getValue({3.0, 2.0, 4.1, 9.0, 4.0, 6.3, 9.2}, true).value);
}

void flowPP_case20(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1020#issue-1375648891
    ClassAnalogDigitalMeter meter(4, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 269.9227,
                             meter.getValue({0.0, 2.0, 6.1, 9.2, 9.0, 2.5, 2.9, 7.2}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 269.92272,
                             meter.getValue({0.0, 2.0, 6.1, 9.2, 9.0, 2.5, 2.9, 7.2}, true).value);
}

void flowPP_case21(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1028#issuecomment-1250239481
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setNDecimalPlaces(4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 169.3593,
                             meter.getValue({1.1, 6.0, 9.1, 3.0, 5.3, 9.4, 3.5}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 169.35935,
                             meter.getValue({1.1, 6.0, 9.1, 3.0, 5.3, 9.4, 3.5}, true).value);
}

void flowPP_case22(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1028#issuecomment-1250239481
    ClassAnalogDigitalMeter meter(8, 1);
    meter.setNDecimalPlaces(4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 211.0355,
                             meter.getValue({9.8, 9.8, 1.9, 0.9, 0.9, 9.9, 2.9, 4.8, 5.5}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 211.03555,
                             meter.getValue({9.8, 9.8, 1.9, 0.9, 0.9, 9.9, 2.9, 4.8, 5.5}, true).value);
}

void flowPP_case23(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1110#issuecomment-1277425333
    ClassAnalogDigitalMeter meter(3, 3);
    // meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);
    meter.setAnalogDigitalTransitionEnd(9);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 245.938,
                             meter.getValue({2.2, 4.5, 5.9, 9.4, 3.8, 8.6}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 245.9386,
                             meter.getValue({2.2, 4.5, 5.9, 9.4, 3.8, 8.6}, true).value);
}

void flowPP_case24(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1110#issuecomment-1281136057
    // and the following comments

    // last digit seems to be continously rolling
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setNDecimalPlaces(4);
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 170.05287,
                             meter.getValue({1.2, 6.8, 0.0, 0.0, 5.0, 2.8, 8.7}, true).value);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 171.24178,
                             meter.getValue({1.2, 7.0, 1.2, 2.0, 4.0, 1.8, 7.8}, true).value);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 386.05672,
                             meter.getValue({3.0, 8.1, 5.9, 0.0, 5.0, 6.7, 7.2}, true).value);
}

void flowPP_case25(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1020#issue-1375648891
    ClassAnalogDigitalMeter meter(3, 3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 247.204,
                             meter.getValue({2.0, 4.0, 6.8, 2.2, 0.1, 4.5}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 247.2045,
                             meter.getValue({2.0, 4.0, 6.8, 2.2, 0.1, 4.5}, true).value);
}

void flowPP_case26(void)
{
    // https://github.com/jomjol/AI-on-the-edge-device/issues/1110#issue-1391153343
    ClassAnalogDigitalMeter meter(3, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 142.9269,
                             meter.getValue({1.0, 4.0, 2.0, 9.2, 2.5, 6.8, 9.0}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 142.92690,
                             meter.getValue({1.0, 4.0, 2.0, 9.2, 2.5, 6.8, 9.0}, true).value);
}

void flowPP_case27(void)
{
    //  https://github.com/jomjol/AI-on-the-edge-device/issues/1110#issuecomment-1262626388
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setNDecimalPlaces(4);
    // letztes digit läuft mit analog zeiger mit. Im originalcode nur mit early transition gelöst
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 170.0528,
                             meter.getValue({1.2, 6.8, 0.0, 0.0, 5.0, 2.8, 8.7}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 170.05287,
                             meter.getValue({1.2, 6.8, 0.0, 0.0, 5.0, 2.8, 8.7}, true).value);
}

void flowPP_case28(void)
{
    //  from unit tests doFlowPP3
    ClassAnalogDigitalMeter meter(4, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 91.8817,
                             meter.getValue({0.0, 0.0, 9.0, 1.0, 9.0, 8.0, 1.8, 7.4}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 91.88174,
                             meter.getValue({0.0, 0.0, 9.0, 1.0, 9.0, 8.0, 1.8, 7.4}, true).value);
}

void flowPP_case29(void)
{
    //  from unit tests doFlowPP3
    ClassAnalogDigitalMeter meter(4, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 92.3832,
                             meter.getValue({0.0, 0.0, 9.0, 1.9, 3.6, 8.2, 3.2, 2.0}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 92.38320,
                             meter.getValue({0.0, 0.0, 9.0, 1.9, 3.6, 8.2, 3.2, 2.0}, true).value);
}

void flowPP_case30(void)
{
    //  from unit tests doFlowPP3
    ClassAnalogDigitalMeter meter(7, 0);
    meter.setNDecimalPlaces(3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 7472.759,
                             meter.getValue({7.0, 4.0, 7.0, 2.0, 7.0, 5.4, 9.4}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 7472.7594,
                             meter.getValue({7.0, 4.0, 7.0, 2.0, 7.0, 5.4, 9.4}, true).value);
}

void flowPP_case31(void)
{
    //  https://github.com/jomjol/AI-on-the-edge-device/issues/1143#issuecomment-1274434805
    ClassAnalogDigitalMeter meter(3, 4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 577.8649,
                             meter.getValue({4.9, 6.9, 6.8, 8.6, 6.2, 5.0, 9.0}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 577.86490,
                             meter.getValue({4.9, 6.9, 6.8, 8.6, 6.2, 5.0, 9.0}, true).value);
}

void flowPP_case32(void)
{
    //  from unit tests doFlowPP3
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setNDecimalPlaces(4);

    // last digit hangs just before 5
    // if the digit is keeping hanging, it might be detuned, so we would need to adapt the u_deviation parameter
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 211.0358,
                             meter.getValue({2.0, 1.0, 1.0, 0.0, 3.0, 4.8, 8.0}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 211.03580,
                             meter.getValue({2.0, 1.0, 1.0, 0.0, 3.0, 4.8, 8.0}, true).value);
}

void flowPP_case33(void)
{
    //  https://github.com/jomjol/AI-on-the-edge-device/issues/1143#issuecomment-1281231468
    ClassAnalogDigitalMeter meter(3, 3);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 126.923,
                             meter.getValue({1.0, 1.9, 6.0, 9.3, 2.3, 3.1}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 126.9231,
                             meter.getValue({1.0, 1.9, 6.0, 9.3, 2.3, 3.1}, true).value);
}

void flowPP_case34(void)
{
    //  https://github.com/jomjol/AI-on-the-edge-device/issues/1110#issuecomment-1282168030
    ClassAnalogDigitalMeter meter(6, 1);
    meter.setLastDigitTransitionMode(ClassAnalogDigitalMeter::Rolling);
    meter.setNDecimalPlaces(4);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 386.0567,
                             meter.getValue({3.0, 8.1, 5.9, 0.0, 5.0, 6.7, 7.2}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 386.05672,
                             meter.getValue({3.0, 8.1, 5.9, 0.0, 5.0, 6.7, 7.2}, true).value);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 171.2417,
                             meter.getValue({1.2, 7.0, 1.2, 2.0, 4.0, 1.8, 7.8}).value);

    // with extended resolution
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 171.24178,
                             meter.getValue({1.2, 7.0, 1.2, 2.0, 4.0, 1.8, 7.8}, true).value);
}

void flowPP_case35(void)
{
    ClassAnalogDigitalMeter meter(5, 4);

    // https://github.com/jomjol/AI-on-the-edge-device/issues/1503#issue-1479304575
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 716.0948,
                             meter.getValue({0.0, 0.0, 6.9, 1.0, 5.8, 0.7, 9.4, 5.2, 8.7}).value);

    // https://github.com/jomjol/AI-on-the-edge-device/issues/1503#issuecomment-1343335855
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 717.01658,
                             meter.getValue({0.0, 0.0, 6.9, 1.0, 6.6, 9.9, 1.8, 6.6, 5.8}, true).value);
}


void runFlowPP()
{
    RUN_TEST(flowPP_case01);
    RUN_TEST(flowPP_case02);
    RUN_TEST(flowPP_case03);
    RUN_TEST(flowPP_case04);
    RUN_TEST(flowPP_case05);
    RUN_TEST(flowPP_case06);
    RUN_TEST(flowPP_case07);
    RUN_TEST(flowPP_case08);
    RUN_TEST(flowPP_case09);
    RUN_TEST(flowPP_case10);
    RUN_TEST(flowPP_case11);
    RUN_TEST(flowPP_case12);
    RUN_TEST(flowPP_case13);
    RUN_TEST(flowPP_case14);
    RUN_TEST(flowPP_case15);
    RUN_TEST(flowPP_case16);
    RUN_TEST(flowPP_case17);
    RUN_TEST(flowPP_case18);
    RUN_TEST(flowPP_case19);
    RUN_TEST(flowPP_case20);
    RUN_TEST(flowPP_case21);
    RUN_TEST(flowPP_case22);
    RUN_TEST(flowPP_case23);
    RUN_TEST(flowPP_case24);
    RUN_TEST(flowPP_case25);
    RUN_TEST(flowPP_case26);
    RUN_TEST(flowPP_case27);
    RUN_TEST(flowPP_case28);
    RUN_TEST(flowPP_case29);
    RUN_TEST(flowPP_case30);
    RUN_TEST(flowPP_case31);
    RUN_TEST(flowPP_case32);
    RUN_TEST(flowPP_case33);
    RUN_TEST(flowPP_case34);
    RUN_TEST(flowPP_case35);
}

/**
 * A very confusing case of late transition
 *
 * This is a real world case of MY own water meter, where the last digit starts transitioning,
 * when the first dial is at around 3.0
 *
 * This tests shows increasing true meter values. Hence, also the expected values need to
 * increase from test to test, even though, some tests seem to be counterintuitive (case 1 and 2).
 */
static ClassAnalogDigitalMeter flowLate_createMeter(void)
{
    ClassAnalogDigitalMeter meter(3, 4);
    meter.setAnalogDigitalTransitionEnd(4.0f);
    return meter;
}


void flowLate_case01(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 12.0210,
                             meter.getValue({0.0, 1.0, 1.0, 0.2, 2.2, 1.0, 0.0}).value);
}

void flowLate_case02(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 12.3210,
                             meter.getValue({0.0, 1.0, 1.2, 3.3, 2.2, 1.0, 0.0}).value);
}

void flowLate_case03(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 12.4210,
                             meter.getValue({0.0, 1.0, 2.0, 4.3, 2.2, 1.0, 0.0}).value);
}

void flowLate_case04(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 12.9870,
                             meter.getValue({0.0, 1.0, 2.0, 9.8, 8.7, 7.0, 0.0}).value);
}

void flowLate_case05(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 13.003,
                             meter.getValue({0.0, 1.0, 2.0, 0.1, 0.3, 3.1, 0.1}).value);
}

void flowLate_case06(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 13.351,
                             meter.getValue({0.0, 1.0, 2.8, 3.5, 5.2, 1.1, 0.1}).value);
}

void flowLate_case07(void)
{
    auto meter = flowLate_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 13.421,
                             meter.getValue({0.0, 1.0, 3.0, 4.1, 2.2, 1.1, 0.1}).value);
}


void runFlowLate()
{
    RUN_TEST(flowLate_case01);
    RUN_TEST(flowLate_case02);
    RUN_TEST(flowLate_case03);
    RUN_TEST(flowLate_case04);
    RUN_TEST(flowLate_case05);
    RUN_TEST(flowLate_case06);
    RUN_TEST(flowLate_case07);
}

/**
 * Early transition test suite.
 *
 * The meter transitions early: the analog→digital transition end is 8.0.
 */
static ClassAnalogDigitalMeter flowEarly_createMeter(void)
{
    ClassAnalogDigitalMeter meter(3, 4);
    meter.setAnalogDigitalTransitionEnd(8.0f);
    return meter;
}

void flowEarly_case01(void)
{
    auto meter = flowEarly_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(
        1e-5, 12.6789,
        meter.getValue({0.0, 1.0, 2.0, 6.7, 7.8, 8.9, 9.0}).value);
}

void flowEarly_case02(void)
{
    auto meter = flowEarly_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(
        1e-5, 12.7234,
        meter.getValue({0.0, 1.0, 2.4, 7.2, 2.3, 3.4, 4.0}).value);
}

void flowEarly_case03(void)
{
    auto meter = flowEarly_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(
        1e-5, 12.7789,
        meter.getValue({0.0, 1.0, 2.7, 7.7, 7.8, 8.9, 9.0}).value);
}

void flowEarly_case04(void)
{
    auto meter = flowEarly_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(
        1e-5, 12.8123,
        meter.getValue({0.0, 1.0, 3.0, 8.1, 1.2, 2.3, 3.0}).value);
}

void flowEarly_case05(void)
{
    auto meter = flowEarly_createMeter();
    TEST_ASSERT_FLOAT_WITHIN(
        1e-5, 13.1234,
        meter.getValue({0.0, 1.0, 3.0, 1.2, 2.3, 3.4, 4.0}).value);
}

void runFlowEarly()
{
    RUN_TEST(flowEarly_case01);
    RUN_TEST(flowEarly_case02);
    RUN_TEST(flowEarly_case03);
    RUN_TEST(flowEarly_case04);
    RUN_TEST(flowEarly_case05);
}


int main()
{
    UNITY_BEGIN();

    runFlowPP();
    runFlowLate();
    runFlowEarly();

    return UNITY_END();

}


