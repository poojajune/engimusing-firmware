#include "Arduino.h"

#include <MqttHub.h>
#include <MqttPort.h>
#include <MqttModule.h>

/*
  EFMUSB Commands:
  {"TOP":"EFMUSB/RED/LED/CTL","PLD":"ON"}
  {"TOP":"EFMUSB/RED/LED/CTL","PLD":"OFF"}
  {"TOP":"EFMUSB/RED/LED/CTL","PLD":"STATUS"}

  {"TOP":"EFMUSB/GREEN/LED/CTL","PLD":"ON"}
  {"TOP":"EFMUSB/GREEN/LED/CTL","PLD":"OFF"}
  {"TOP":"EFMUSB/GREEN/LED/CTL","PLD":"STATUS"}

  {"TOP":"EFMUSB/BLUE/LED/CTL","PLD":"ON"}
  {"TOP":"EFMUSB/BLUE/LED/CTL","PLD":"OFF"}
  {"TOP":"EFMUSB/BLUE/LED/CTL","PLD":"STATUS"}

  {"TOP":"EFMUSB/CPU/TMPC"}
  {"TOP":"EFMUSB/CPU/TMPF"}
  {"TOP":"EFMUSB/CPU/VDD"}
*/

MqttHub HUB;
MqttSerialPort serialPort;

// LED pin numbers
#define GREEN_LED    13
#define BLUE_LED     14
#define RED_LED      15

// Instantiate classes for each the LEDs on the EFMUSB CPU board
// The OnOffCtlModule allows for turning the LEDs on and off and
// requesting their status

OnOffCtlModule EFMREDLED;
OnOffCtlModule EFMBLUELED;
OnOffCtlModule EFMGREENLED;

// Instantiate classes for the CPU temperature and supply voltage
// These classes can be used to read the values or can be set up
// to periodically report the values

CpuVddModule EFMCPUVDD;
CpuTempModule EFMCPUTMP;

void setup()
{
  serialPort.begin(HUB, Serial);

  // Start each of the LED services
  // The first parameter is the LED pin
  // The second parameter is the module name
  // The third parameter is the on state for the LEDs
  EFMREDLED.begin(HUB, RED_LED, "EFMUSB/RED/LED", LOW);
  EFMBLUELED.begin(HUB, BLUE_LED, "EFMUSB/BLUE/LED", LOW);
  EFMGREENLED.begin(HUB, GREEN_LED, "EFMUSB/GREEN/LED", LOW);

  // Start each of the CPU services
  // The first parameter is the module name
  // The other parameters are time values in tenths of seconds
  // The CPUVDD will send a status every 10 seconds
  EFMCPUVDD.begin(HUB, "EFMUSB/CPU", 100);
  // The CPUTEMP has two possible status options
  // The first number is 5 seconds for a Farenheit reading
  // The second number is 5 seconds for a Celcius reading
  EFMCPUTMP.begin(HUB, "EFMUSB/CPU", 50, 50);
}

void loop()
{
  HUB.update();
}
