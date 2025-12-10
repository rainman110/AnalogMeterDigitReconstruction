#include <iostream>
#include "src/ClassAnalogDigitalMeter.h"

#include "tests/test_watermeterhelpers.h"

#include "unity.h"

using namespace std;






int main()
{

    UNITY_BEGIN();
    runHelperTests();
    return UNITY_END();
}
