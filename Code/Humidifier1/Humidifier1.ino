/*    
 * Humidifier.c 
 * This is a demo for Water  
 *   
 * Copyright (c) 2015 seeed technology inc.  
 * Author      : Jiankai Li  
 * Create Time:  Oct 2015
 * Change Log : 
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <Wire.h>
#include <I2C_LCD.h>
#include "DHT.h"
#define DEBUG 1

enum status{
  Standby            = 0,
  Humidification     = 1,
  AutoHumidification = 2,
  
};
typedef enum status SystemStatus;
SystemStatus WorkingStatus;

I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

#define DHTPIN A0     // what pin we're connected to
// Uncomment whatever type you're using!
#define OneSecond     1000
#define DHTTYPE       DHT11   // DHT 11 
#define WaterAtomPin  5
#define ButtonPin     2
#define DateUpdateInterval 5   // unit seconds
#define HumidityLimen 50
uint8_t StartTime;
uint8_t ButtonFlag;
//For detials of the function useage, please refer to "I2C_LCD User Manual". 
//You can download the "I2C_LCD User Manual" from I2C_LCD WIKI page: http://www.seeedstudio.com/wiki/I2C_LCD

//The usage of LCD.print() is same to the Serial.print().
//For more informations about print, please visit: http://www.arduino.cc/en/Serial/Print
DHT dht(DHTPIN, DHTTYPE);
void setup(void)
{
    Wire.begin();         //I2C controller initialization.
    LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    LCD.WorkingModeConf(ON, ON, WM_CharMode);
    LCD.BacklightConf(LOAD_TO_RAM, 20);
    
    LCD.FontModeConf(Font_10x20, FM_ANL_AAA, BLACK_BAC);
    LCD.DispStringAt("SeeedStudio", 10, 20);    //Display "Sparking" on coordinate of (0, 20).
    delay(3000); 
    LCD.CleanAll(WHITE);    //Clean the screen with black or white.
#if DEBUG
    Serial.begin(9600); 
    Serial.println("Enable Debug");
#endif
    pinMode(WaterAtomPin,OUTPUT);
    pinMode(ButtonPin,INPUT);
    attachInterrupt(0,Buttonclick,RISING);
    dht.begin();
    DisableWaterAtom();
    WorkingStatus = Standby;
    StartTime = millis()/OneSecond + DateUpdateInterval;
}

void loop(void)
{
    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();
    switch (WorkingStatus) {
    case Standby:
        if(((millis()/OneSecond) - StartTime) > DateUpdateInterval) {
            if (isnan(temperature) || isnan(humidity)) {
                Serial.println("Failed to read from DHT");
            } else {
                LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

                //Set the start coordinate.
                LCD.CharGotoXY(0,0);

                //Print string on I2C_LCD.
                LCD.print("Temp: ");
                //Print float on I2C_LCD.
                LCD.print(((float)((int)(temperature*10))/10), 1);
                LCD.println("`C");  
                LCD.print("Hum : ");
                //Print float on I2C_LCD.
                LCD.print(((float)((int)(humidity*10))/10), 1);
                LCD.println(" %");    
            
                //Print the number of seconds since reset:
                LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
                LCD.CharGotoXY(0,48);
                LCD.print("Hum Limen : ");
                LCD.print(HumidityLimen);
                LCD.println(" %");
                //LCD.CharGotoXY(0,56);
                LCD.println("Prs BTN to moisturize");
        
                //LCD.CharGotoXY(68,48);
                //Print the number of seconds since reset:
                //LCD.print(millis()/1000,DEC);
                StartTime = millis()/OneSecond;
        }
        }
#if DEBUG
            Serial.println("StandBy");
#endif
        if (ButtonFlag == true) {
            delay(200);
            ButtonFlag = false;
            WorkingStatus = Humidification;
            LCD.CleanAll(WHITE);
        }
        if (humidity <= HumidityLimen) {
            WorkingStatus = AutoHumidification;
        }
        break;
    case Humidification: 
        if(ButtonFlag == true) {
            delay(200);
            ButtonFlag = false;
            WorkingStatus = Standby;
            LCD.CleanAll(WHITE);
            DisableWaterAtom();
        } else {
            EnableWaterAtom();
            LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

                //Set the start coordinate.
            LCD.CharGotoXY(0,0);

                //Print string on I2C_LCD.
            LCD.print("Temp: ");
                //Print float on I2C_LCD.
            LCD.print(((float)((int)(temperature*10))/10), 1);
            LCD.println("`C");  
            LCD.print("Hum : ");
                //Print float on I2C_LCD.
            LCD.print(((float)((int)(humidity*10))/10), 1);
            LCD.println(" %");   
            LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
            LCD.CharGotoXY(0,48);
            LCD.println("Prs BTN to STOP");
        }
        
#if DEBUG
    Serial.println("Humidification");
#endif
        break;
    case AutoHumidification:
        if(ButtonFlag == true) {
            delay(200);
            ButtonFlag = false;
            WorkingStatus = Standby;
            LCD.CleanAll(WHITE);
            DisableWaterAtom();
        } else {
            EnableWaterAtom();
            LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

                //Set the start coordinate.
            LCD.CharGotoXY(0,0);

                //Print string on I2C_LCD.
            LCD.print("Temp: ");
                //Print float on I2C_LCD.
            LCD.print(((float)((int)(temperature*10))/10), 1);
            LCD.println("`C");  
            LCD.print("Hum : ");
                //Print float on I2C_LCD.
            LCD.print(((float)((int)(humidity*10))/10), 1);
            LCD.println(" %");   
            LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
            LCD.CharGotoXY(0,48);
            LCD.println("Prs BTN to STOP");
        }
        if (humidity > HumidityLimen) {
            WorkingStatus = Standby;
            LCD.CleanAll(WHITE);
            DisableWaterAtom();
        }
        break;
    default:
        break;
        
    }
    
}


void EnableWaterAtom()
{
    digitalWrite(WaterAtomPin, HIGH);
}
void DisableWaterAtom()
{
    digitalWrite(WaterAtomPin, LOW);
}


void Buttonclick()
{
    ButtonFlag = true;
#if DEBUG
    Serial.println("Buttonclick");
#endif
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/






