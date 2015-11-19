#include "Arduino.h"
#include "test.h"

extern LEUARTClass Serial;
static String test_help = "test14 - help entry";

void test14_test(void)
{
}

void test14_desc(void)
{
  delay(2); Serial.println("\n\rTest 14 Description:");
  delay(2); Serial.println("  Long multi-line description of the test");
  delay(2); Serial.println("  ");
}

void test14(uint8_t function)
{
  switch(function) {
  case TESTHELP:
    delay(2); Serial.print(test_help);
    break;
  case DESCRIPTION:
    test14_desc();
    break;
  case RUNTEST:
    Serial.println("\n\rRun test14");
    test14_test();
    break;
  default:
    Serial.println("\n\rBad Test Function");
    break;
  }
}
