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
/* Example for how to setup the MQTT client for the TMP102 RS232x2 Engimusing board
 *  There are 2 devices on this board. An LED and a TMP102 temperature sensor. 
 *  See http://www.engimusing.com/products/tmp-3 for more information about the board.
 */

//Include the EFM32COMM to get the MQTT client classes
#include <EFM32COMM.h>

#include <Wire.h>
/*
  EFMZG108 Commands:
  {"TOP":"EFMZG108/BOARD/LED/CTL","PLD":"ON"}
  {"TOP":"EFMZG108/BOARD/LED/CTL","PLD":"OFF"}
  {"TOP":"EFMZG108/BOARD/LED/CTL","PLD":"STATUS"}

  {"TOP":"EFMZG108/BOARD/TMP102/DEC_C","PLD":"STATUS"}
*/

//MQTT class defintions
// The EFM32COMM classes are automatically registered with the COMM 
// object when begin() is called so they can be updated 
// whenever COMM.update() is called.
onOffCtlClass LEDCtrl;

tmp102Class TMP102;

void setup()
{
  Serial.begin(115200);

  //Initialize the on off control to connect it to
  // the LED that is on the board
  LEDCtrl.begin(13, "EFMZG108/BOARD/LED", HIGH);

  //Initialize the tmp control class which will send the 
  // temperature over MQTT every 10 seconds
  TMP102.begin("EFMZG108/BOARD/TMP102", &Wire0, -1, 10000);
}

//Part of light on off example
//int lastMillisOn = 0;
//int lastMillisOff = 1000;

void loop()
{
  //Update the MQTT communication so it
  // can send statuses and recieve requests
  COMM.update();

  /*
  //example of how to turn on and off a light using the onOffCtlClass
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
