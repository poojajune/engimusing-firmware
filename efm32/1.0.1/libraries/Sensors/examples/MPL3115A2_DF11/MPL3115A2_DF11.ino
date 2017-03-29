/*
  Copyright (c) 2016-2017 Engimusing LLC.  All right reserved.

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
/* Example for how to print out readings from the MPLA3115A RS232x2 Engimusing board
 *  There are 3 devices on this board. An LED and an MPLA3115A temperature and altitude sensor. 
 *  See http://www.engimusing.com/products/reedsw-1 for more information about the board.
 */

#if !defined(EFM32ZGUSB)
#error Incorrect Board Selected! Please select Engimusing EFM32ZGUSB from the Tools->Board: menu.
#endif

#include "Arduino.h"

#include <MPLA3115A2Device.h>
#include <Wire.h>

MPLA3115A2Device MPLA3115A2;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Initialize the sensor
  MPLA3115A2.begin(Wire0, 5);

  delay(1000);
  Serial.println("Simple MPLA3115A2 DF11 example 0");
}


void loop()
{
  static int on = LOW;

  digitalWrite(LED_BUILTIN, on);   // turn the LED on (HIGH is the voltage level)
  
  float pressue = MPLA3115A2.readPressure();
  float altitude = MPLA3115A2.readAltitude();
  float temp = MPLA3115A2.readTemp();
  Serial.print("Pressure = ");
  Serial.print(pressue);
  Serial.print(" Pa Altitude = ");
  Serial.print(altitude);
  Serial.print(" M temperature = ");
  Serial.print(temp);
  Serial.println(" C");
  
  delay(500);                       // wait for a second
  
  on = (on) ? LOW : HIGH;  // on alternates between LOW and HIGH
  
}