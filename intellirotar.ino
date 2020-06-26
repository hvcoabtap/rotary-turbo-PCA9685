#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
uint32_t uint32CurrentRate = 1000;

//Rotary encoder
#define UBINCLK_STATE ((bool)digitalRead(PB13))
#define UBINBDATA_VALUE ((bool)digitalRead(PB14)) //unsigned binary boolean data value
#define UBINBSWITCH_VALUE ((bool)digitalRead(PB15))//unsigned binary boolean switch value
volatile bool blRETriggered = false;
uint32_t uint32RETriggeredInstant = 0;
volatile bool blSwitchTriggered = false;
uint32_t uint32SwitchPressInstant = 0;

int16_t int16RotaryEncoderCount = 0;
int16_t int16RotaryEncoderCountDelta = 0;
volatile bool blCLKPinState;
volatile bool blCLKPinLastState;
volatile bool blREClockJustTriggered = false;
volatile bool blREFastMode = false;
uint32_t uint32RETriggeredDeltaMillis = 0;
volatile bool blREClockwise = false;
uint32_t uint32FastModeTimeStart = 0;

int16_t int16Range = 500;
const uint16_t uint16PWMRersolution = 4096 - 1;//0 to 4095 for luminosity
uint16_t uint16RotaryVal = 0;
uint16_t uint16LogValToSet = 0;

HardwareSerial Serial2(USART2);//USART2 NO 5V tolerant, to ---> FTDI ---> PC


void setup()
{

  Serial2.begin(230400);
  //Rotary
  pinMode(PB13, INPUT);//PIN A, CLK //pullup present in board // +capacitors to pin and ground for all tree pins; each 0,1 mF (100 nF)
  pinMode(PB14, INPUT);//PIN B, (DT) //pullup present in board
  pinMode(PB15, INPUT_PULLUP);//SWITCH //important! pullup resistor (not present in board) need avoiding capacitor charge (if pressed, remain as "pressed"!)
  blCLKPinLastState = UBINCLK_STATE;
  attachInterrupt(digitalPinToInterrupt(PB13), RETriggered, CHANGE);//Rotary Encoder clock value change event

  //PCA9685
  pwm.begin();
  pwm.setPWMFreq(1600);
}


void RETriggered()
{
  uint32RETriggeredInstant = millis();//reset trigger instant
  if (!blREClockJustTriggered)
  {
    int16RotaryEncoderCountDelta = 0;
  }
  blRETriggered = true;
  blCLKPinState = UBINCLK_STATE;
  if (blCLKPinState != blCLKPinLastState)
  {
    if (UBINBDATA_VALUE != blCLKPinState)
    {
      blREClockwise = true;
      int16RotaryEncoderCount++;
    }
    else
    {
      blREClockwise = false;
      int16RotaryEncoderCount--;
    }
    blCLKPinLastState = blCLKPinState;//blCLKPinState is first state in changing direction
    if (int16RotaryEncoderCount < 0)//limits
      int16RotaryEncoderCount = 0;
    if (int16RotaryEncoderCount > (int16Range - 1))
      int16RotaryEncoderCount = int16Range - 1;

    uint16RotaryVal = int16RotaryEncoderCount;
    blREClockJustTriggered = true;//Event status
    if (blREClockJustTriggered)
    {
      int16RotaryEncoderCountDelta++;
    }
  }
}

void loop()
{
  uint32RETriggeredDeltaMillis = millis() - uint32RETriggeredInstant;
  if (uint32RETriggeredDeltaMillis > 3000)
  {
    int16RotaryEncoderCountDelta = 0;
    blREClockJustTriggered = false;
  }
  if ((uint32RETriggeredDeltaMillis > 50) && blREClockJustTriggered)//if each minimum 50 milliseconds
  {
    blREClockJustTriggered = false;       //checking fast mode switch
    if (!blREFastMode)//check for auto switching to fast mode
    {
      if (int16RotaryEncoderCountDelta > 8)
      {
        blREFastMode = true;
        Serial2.println("FAST");
        uint32FastModeTimeStart = millis();
        int16RotaryEncoderCountDelta = 0;
      }
    }
  }


  if (((millis() - uint32FastModeTimeStart) > 3000) && blREFastMode)// 3secs interval for fast mode
  {
    blREFastMode = false;                 //fast mode switch
    int16RotaryEncoderCountDelta = 0;
    Serial2.println("SLOW");
  }
  if (blRETriggered)
  {
    if (blREFastMode)//tuning fast
    {
      if (blREClockwise)
      {
        int16RotaryEncoderCount += 11;
      }
      else
      {
        int16RotaryEncoderCount -= 11;
      }
      if (int16RotaryEncoderCount < 0)//limits
        int16RotaryEncoderCount = 0;
      if (int16RotaryEncoderCount > (int16Range - 1))
        int16RotaryEncoderCount = int16Range - 1;
      uint16RotaryVal = int16RotaryEncoderCount;

    }

    uint16LogValToSet = uint16PWMRersolution - log((double)(int16Range - uint16RotaryVal)) * uint16PWMRersolution / log(int16Range);

    Serial2.print(blREClockJustTriggered);
    Serial2.print(" ");
    Serial2.print(blREFastMode);
    Serial2.print(" ");
    Serial2.print(int16RotaryEncoderCount);
    Serial2.print(" ");
    Serial2.print(uint16RotaryVal);
    Serial2.print(" ");
    Serial2.println(uint16LogValToSet);
    Serial2.println(int16RotaryEncoderCountDelta);
    pwm.setPWM(0, 0, uint16LogValToSet);
    blRETriggered = false;
  }

  uint8_t intBit = 0;
  uint32_t intBridge = 0;
  String strCommand = "0";
  String strBridge = "0";
  if (Serial2.available() > 0)
  {
    strCommand = Serial2.readString();
    delay(100);
    //if setPWM
    if (strCommand.indexOf("*") != -1)
    {
      strBridge = strCommand.substring(0, strCommand.indexOf('*'));
      intBit = strBridge.toInt();
      strBridge = strCommand.substring(strCommand.indexOf('*') + 1);
      intBridge = strBridge.toInt();
      pwm.setPWM(intBit, 0, intBridge);
    }
    if (strCommand.indexOf("R") != -1)//R is pulse rate, as "framerate"
    {
      strBridge = strCommand.substring(strCommand.indexOf('R') + 1);
      intBridge = strBridge.toInt();
      pwm.setPWMFreq(intBridge);
      uint32CurrentRate = intBridge;
    }
  }

}


