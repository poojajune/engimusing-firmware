/*
  Copyright (c) 2016 Engimusing LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/* Example for how to setup the MQTT client for the ACS716 DF11 board using the ZB USB Engimusing board
 *  There are 2 devices using these combinations of boards. An LED and an ACS716 current sensor. 
 *  See http://www.engimusing.com/products/ACS716-2 for more information about the board.
 */

#if !defined(EFM32ZGUSB)
#error Incorrect Board Selected! Please select Engimusing EFM32ZGUSB from the Tools->Board: menu.
#endif
 
//Include the MqttModule to get the MQTT client classes
#include <MqttHub.h>
#include <MqttPort.h>
#include <MqttModule.h>

#include <ACS716Device.h>

/*
  EFMZGUSB Commands:
  {"TOP":"EFMZGUSB/BOARD/LED/CTL","PLD":"ON"}
  {"TOP":"EFMZGUSB/BOARD/LED/CTL","PLD":"OFF"}
  {"TOP":"EFMZGUSB/BOARD/LED/CTL","PLD":"STATUS"}

  {"TOP":"EFMZGUSB/BOARD/ACS716/CURRENT","PLD":"STATUS"}
*/

MqttHub HUB;
MqttSerialPort serialPort;

//MQTT class defintions
// The MqttModule classes are automatically registered with the COMM 
// object when begin() is called so they can be updated 
// whenever HUB.update() is called.
OnOffCtlModule LEDCtrl;

ACS716Device ACS716;
SimpleMqttModule ACS716MqttMod;

void setup()
{
  serialPort.begin(HUB, Serial);

  //Initialize the on off control to connect it to
  // the LED that is on the board
  LEDCtrl.begin(HUB, 13, "EFMZGUSB/BOARD/LED", LOW);

  //Initialize the ACS716 which will report the current every 10 seconds
  // The two parameters A1 are the power pin and power feedback pin which in this case are the same
  // other implementations they may not be the same and can both be -1 which means neither is connected
  // The A0 parameter is the ADC pin that is connected to the ACS716 output pin.
  ACS716.begin(ACS716Device::ACS716_6BB, A1, A1, A0, 100);
  ACS716MqttMod.begin(HUB, ACS716, "EFMZGUSB/BOARD/ACS716", 10000);
}

//Part of light on off example
//int lastMillisOn = 0;
//int lastMillisOff = 1000;

void loop()
{
  //Update the MQTT communication so it
  // can send statuses and recieve requests
  HUB.update();

  /*
  //example of how to turn on and off a light using the OnOffCtlModule
  //status of the pin will be sent to the MQTT broker.
  if(millis() > lastMillisOff + 2000)
  {
    LEDCtrl.setPinState(LOW);
    lastMillisOff = millis();
  }
  if(millis() > lastMillisOn + 2000)
  {
    LEDCtrl.setPinState(HIGH);
    lastMillisOn = millis();
  }
  */

  /*
   * example of how to control the LED using one of the reed switches
   */
   /*
   //If the led is not in the same state as the swich then 
   // set it to the same as the swich. This check avoids
   // setting the led state every time through the loop
   if(LEDCtrl.pinState() != ReedSwitch0.switchState())
   {
      LEDCtrl.setPinState(ReedSwitch0.switchState());
   }*/
  
  
}