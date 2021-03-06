/*
  Copyright (c) 2017 Engimusing LLC.  All right reserved.
  
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

#include <Arduino.h>
#include "toggle.h"

TOGGLEClass::TOGGLEClass() 
    : myPin(-1)
{
}

void TOGGLEClass::begin(int toggleTime, uint8_t activeLevel)
{
  TOGGLEClass::begin(LED_BUILTIN, OUTPUT, toggleTime, toggleTime, activeLevel);
}

void TOGGLEClass::begin(uint32_t dwPin, int lowTime, int highTime, uint8_t activeLevel)
{
  TOGGLEClass::begin(dwPin, OUTPUT, lowTime, highTime, activeLevel);
}

void TOGGLEClass::begin(uint32_t dwPin, WiringModeTypeDef dwMode, int lowTime, int highTime, uint8_t activeLevel)
{
  myPin = dwPin;
  pinMode(myPin, dwMode);
  myHiTime = highTime;
  myLoTime = lowTime;
  myLastMillis = 0;
  myOn = HIGH;
  myActiveLevel = activeLevel;
}

void TOGGLEClass::update(void)
{
    if(myPin >= 0) // make sure begin has been called with a resonable pin
    {
        if(myOn == HIGH) {
            if(millis() - myLastMillis > myHiTime) {
                myLastMillis = millis();
                if(myActiveLevel == HIGH)
                {
                    digitalWrite(myPin, LOW);
                }
                else
                {
                    digitalWrite(myPin, HIGH);
                }
                myOn = LOW;
            }
        }
        if(myOn == LOW) {
            if (millis() - myLastMillis > myLoTime) {
                myLastMillis = millis();
                digitalWrite(myPin, myActiveLevel);
                myOn = HIGH;
            }
        }
    }
}

void TOGGLEClass::setToggleTime(int time) 
{ 
    setLowHiTimes(time, time);
}

void TOGGLEClass::setLowHiTimes(int loTime, int hiTime)
{
    myLoTime = loTime;
    myHiTime = hiTime;
}

int TOGGLEClass::lowTime()
{
    return myLoTime;
}

int TOGGLEClass::highTime()
{
    return myHiTime;
}
  

