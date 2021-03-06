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

#pragma once

#include "Arduino.h"
#include "MqttHub.h"

class Device;

//Base class for modules, modules are updated periotically and can handle messages recieved from a port
class MqttModule
{
   
  friend class MqttHub;
  
 public:
  
  virtual void begin(MqttHub &hub, const char* module, bool subOnHeartbeat = true);
  virtual void update(void);
  virtual uint8_t decode(const char* topic, const char* payload);
 
 protected:
  uint8_t isTopicThisModule(const char* topic);
  int8_t compare_token(const char* inTok, const char* cmpTok);

 protected:
  MqttHub *myHub;
  const char* myModule;
  bool mySubOnHeartbeat;
  uint32_t myTick;
  MqttModule* myNextModule;
};

// ------------------------------- Message Input Class -------------------------
template <typename T>
class MessageInputModule : public MqttModule
{
 public:
  
  virtual void begin(MqttHub &hub, const char* module, const char* control)
  {
      MqttModule::begin(hub, module, true);

      myControl = control;
      myInputValue = 0;
   }
  
  virtual uint8_t decode(const char* topic, const char* payload)
  {
      int8_t j = isTopicThisModule(topic);
      if(j == 0)
      {
        return 0;
      }  
      
      if(compare_token(&topic[j],myControl)) {
          parsePayload(payload, myInputValue);
          return 1;
      }
  }

  
  virtual T getInput(void)
  {
    return myInputValue;
  }

 protected:
 
  virtual void parsePayload(const char* payload, float &output)
  {
      output = atofLocal(payload);
  }
  
  virtual void parsePayload(const char* payload, int32_t &output)
  {
      output = atoi(payload);
  }
  
  virtual void parsePayload(const char* payload, bool &output)
  {
      if(payload[0] == '0')
      {
        output = false;
      }
      output = true;
  }

  T myInputValue;
  const char* myControl;

};

// ------------------------------- Message Input Class -------------------------
class MessageInputStringModule : public MqttModule
{
 public:
  virtual void begin(MqttHub &hub, const char* module, const char* control);
  virtual uint8_t decode(const char* topic, const char* payload);
  virtual const char *getInputString(void);

 private:
  static const int maxStringSize = 64;
  char myInputString[maxStringSize];
  const char* myControl;

};


// ------------------------------- Notification Class -------------------------

class NotificationModule : public MqttModule
{
 public:
  
  virtual void begin(MqttHub &hub, const char* module, const char* control, const char* payload);
  virtual uint8_t decode(const char* topic, const char* payload);
  virtual uint8_t getState(void);

 private:

  uint8_t myState;
  const char* myControl;
  const char* myPayload;
  
};

// ------------------------------- On/Off Control Class -------------------------

class OnOffCtlModule : public MqttModule
{
 public:
  
  virtual void begin(MqttHub &hub, uint8_t pin, const char* module, uint8_t active);
  virtual uint8_t decode(const char* topic, const char* payload);
  
  virtual void setPinState(uint8_t on);
  virtual uint8_t pinState();
 
 private:
  
  uint32_t myPin;
  uint8_t myActive;
  uint8_t myOn;
  uint8_t myOff;
  
};

// --------------------------------- Detector Switch Class -------------------------

class DetectorSwitchModule : public MqttModule
{
 
 public:
  virtual void  begin(MqttHub &hub, uint8_t pin, const char* module, uint8_t bounceCount);
  virtual void  update(void); // publish changes in switch state
  virtual uint8_t decode(const char* topic, const char* payload);
  virtual uint32_t switchState(void);
 
 protected:
 
  void switchMsg(uint8_t currentSwitch);
  
 protected:
  uint32_t myPin;  // connector pin connected to switch
  uint8_t myEventInProgress; // switch event in progress
  uint32_t mySwitchState;
  uint8_t myBounceCnt; // bounce filter value, 0 = no filter, 0xff = momentary
  
};


// ------------------------------- Momentary Switch Class -------------------------

class MomentarySwitchModule : public MqttModule
{
 
 public:
  virtual void begin(MqttHub &hub, uint8_t pin, const char* module, uint8_t bounceCount);
  virtual void update(void); // publish changes in switch state
 
 private:
  uint32_t myPin;  // connector pin connected to switch
  uint8_t myEventInProgress; // switch event in progress
  uint8_t myBounceCnt; // bounce filter value, 0 = no filter, 0xff = momentary

};

// ------------------------------- SimpleMqttModule -------------------------
class SimpleMqttModule : public MqttModule
{
 
   public:
        virtual void begin(MqttHub &hub, Device &device, const char* mod, uint32_t updateDelay);
        virtual void update(void); // publish changes in switch state
        virtual uint8_t decode(const char* topic, const char* payload);
   protected:
        virtual void sendMQTTData();
   protected:
        Device *myDevice;
        uint32_t myUpdateDelay;
      
};

// ------------------------------- ADC Pin Class -------------------------

class AdcCtlModule : public MqttModule
{
 public:
  virtual void begin(MqttHub &hub, uint8_t pin, const char* module, uint32_t interval);
  virtual void update(void);
  virtual uint8_t decode(const char* topic, const char* payload);
 
 private:
  void publishADCvoltage(void);
  
 private:
  uint32_t myCurrent;
  uint32_t myInterval;
  uint32_t myAdcPin;
  
};


// ------------------------------- Tone Control Class -------------------------

class ToneCtlModule : public MqttModule
{
 public:
 
   virtual void begin(MqttHub &hub, uint8_t pin, const char* module);
   virtual uint8_t decode(const char* topic, const char* payload);
 
 private:
 
  uint32_t myPin;
  uint32_t myToneFreq;
  uint32_t myToneDuration;
  uint32_t myToneState;
  TimersLP myTimer;

};


