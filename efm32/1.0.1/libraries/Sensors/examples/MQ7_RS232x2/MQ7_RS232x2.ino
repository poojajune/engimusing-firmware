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
/* Example for how to print out readings from the MQ7 RS232x2 Engimusing board
    There are 2 devices on this board. An LED and a MQ7 CO detector.
    See https://www.engimusing.com/products/gas-4 for more information about the board.
*/

#if !defined(EFM32TG110)
#error Incorrect Board Selected! Please select Engimusing EFM32TG110 from the Tools->Board: menu.
#endif

#include <MQ7Device.h>


MQ7Device MQ7;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.println("Simple MQ7 example 0");
  Serial1.println("Simple MQ7 example 1");

  
  //Initialize the MQ7 CO Sensor
  //Pins:
  // PWM Controller - 7
  // Analog Sensor Input - A0
  
  MQ7.begin(7, A0);
}

int lastMillis = 0; // store the last time the current was printed.
int printDelay = 1000; //print every second.

void loop()
{

  static int on = HIGH;

  MQ7.update();

  if(millis() - lastMillis > printDelay)
  {
    lastMillis = millis();

    digitalWrite(LED_BUILTIN, on); // toggle the LED (HIGH is the voltage level)
    
    uint32_t alertValue = MQ7.currentAlertValue();
    const char * state = MQ7.currentAlertText();
    Serial.print("value = ");
    Serial.print(alertValue);
    Serial.print(" state = ");
    Serial.println(state);
    
    Serial1.print("value = ");
    Serial1.print(alertValue);
    Serial1.print(" state = ");
    Serial1.println(state);

    on = (on) ? LOW : HIGH; // on alternates between LOW and HIGH
  }
}