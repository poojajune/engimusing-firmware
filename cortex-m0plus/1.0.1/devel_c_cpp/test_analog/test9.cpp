#include "Arduino.h"
#include "test.h"

extern LEUARTClass Serial;
static String test_help = "test09 - help entry";

void test09_test(void)
{
}

void test09_desc(void)
{
  delay(2); Serial.println("\n\rTest 09 Description:");
  delay(2); Serial.println("  Long multi-line description of the test");
  delay(2); Serial.println("  ");
}

void test09(uint8_t function)
{
  switch(function) {
  case TESTHELP:
    delay(2); Serial.print(test_help);
    break;
  case DESCRIPTION:
    test09_desc();
    break;
  case RUNTEST:
    Serial.println("\n\rRun test09");
    test09_test();
    break;
  default:
    Serial.println("\n\rBad Test Function");
    break;
  }
}
