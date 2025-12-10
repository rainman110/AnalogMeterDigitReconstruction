#include "ClassAnalogDigitalMeter.h"

#include <unity.h>
#include <unity_internals.h>


void setUp(void) {}
void tearDown(void) {}

void test_circDistance10(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::circDistance10(0, 5), 5.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::circDistance10(5, 0), 5.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::circDistance10(2, 5), 3.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::circDistance10(9, 1), 2.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::circDistance10(1, 9), 2.);
}

void test_decimalPhase(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::decimalPhase(123.4, 0), 0.4);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::decimalPhase(123.4, 1), 0.34);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::decimalPhase(123.4, 2), 0.234);
}

void test_baseDigit(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::baseDigit(123.4, 0), 3.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::baseDigit(123.4, 1), 2.);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, helper::baseDigit(123.4, 2), 1.);
}

void runHelperTests()
{
    RUN_TEST(test_circDistance10);
    RUN_TEST(test_decimalPhase);
    RUN_TEST(test_baseDigit);
}


int main()
{
    UNITY_BEGIN();
    runHelperTests();
    return UNITY_END();
}


