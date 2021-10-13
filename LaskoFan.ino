
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#include "DHT.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h"

// Initializing 
TFT_eSPI tft; // TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); // buffer

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 0
DHT dht(DHTPIN, DHTTYPE);

bool fanOn = 0;

IRsendRaw mySender;

//raw data arrays 
uint16_t POWER[] = {1300, 350, 1300, 400, 400, 1250, 1250, 400, 1250, 400, 450, 1250, 400, 1250, 450, 1200, 450, 1250, 400, 1250, 450, 1200, 1250};
uint16_t FAN_ROTATE[] = {1250, 400, 1250, 400, 450, 1200, 1300, 350, 1250, 450, 400, 1250, 450, 1200, 1300, 400, 400, 1250, 450, 1250, 400, 1250, 450};
uint16_t FAN_SPEED[] = {1250, 400, 1250, 400, 400, 1250, 1250, 400, 1250, 400, 450, 1250, 450, 1200, 450, 1250, 400, 1250, 450, 1200, 1250, 450, 400};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_BUZZER, OUTPUT);

  bool fanOn = 0;

  // setup screen
    tft.begin();
    tft.setRotation(3);
    spr.createSprite(TFT_HEIGHT, TFT_WIDTH);
 
 
}



void loop() {

  //float temp_val;
  char str[10];

  int temp_val = dht.readTemperature(true);
  
  //Setting the title
  spr.fillSprite(TFT_WHITE); //Fill background with white color
  spr.fillRect(0,0,320,50,TFT_PURPLE); //Rectangle fill with dark green 
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setFreeFont(FSSB12);
  spr.drawString("Lasko Fan Automation",30,15,1); //Drawing a text string 

  spr.drawFastVLine(160,50,190,TFT_PURPLE); //Drawing verticle line


  // Temp string 
  spr.setTextColor(TFT_BLACK);
  //spr.setTextSize(2);
  spr.setFreeFont(FS12);
  spr.drawString("Temperature",18,120);
  spr.drawFastHLine(17,142,130,TFT_BLACK); //Drawing horizontal line
  //spr.setTextSize(3);
  // edit 
  spr.setFreeFont(FSB18);
  spr.drawNumber(temp_val,50,150 );
  spr.setFreeFont(FSB18);
  spr.drawString("F",90,150);

  // Fan Status
  spr.setFreeFont(FS12);
  spr.drawString("Fan status", 190,120);
  spr.drawFastHLine(189,142,100,TFT_BLACK); //Drawing horizontal line


// check temp and turn the fan on when its above 76
  if (roundf(temp_val) >= 76)
  {
    Serial.println("Fan on: ");
    Serial.print(fanOn);

    spr.setFreeFont(FSB18);
    spr.drawString("On", 210,150);
    if (fanOn !=1){

      analogWrite(WIO_BUZZER, 128);
      analogWrite(WIO_BUZZER, 120);
      mySender.send(POWER, sizeof(POWER) / sizeof(POWER[0]), 38);
      fanOn = 1;
    }
  } else if (roundf(temp_val) <= 74) // Turn the fan off when its =< 74
    { // if fan was turn on
        Serial.println("Fan off: ");
        Serial.print(fanOn);
        spr.setFreeFont(FSB18);
        spr.drawString("Off", 210,150);
      if (fanOn) 
      { 
        analogWrite(WIO_BUZZER, 128);
        analogWrite(WIO_BUZZER, 120);
        mySender.send(POWER, sizeof(POWER) / sizeof(POWER[0]), 38);
        fanOn = 0;
      }
    }


  // Fan code for the buttons
  if (digitalRead(WIO_KEY_A) == LOW) {
      Serial.println("A Key pressed - Power");
      analogWrite(WIO_BUZZER, 128);
      //mySender.send(POWER, 23);
      mySender.send(POWER, sizeof(POWER) / sizeof(POWER[0]), 38);
    }
    else if (digitalRead(WIO_KEY_B) == LOW) {
      Serial.println("B Key pressed - Speed");
      analogWrite(WIO_BUZZER, 128);
      mySender.send(FAN_SPEED, sizeof(FAN_SPEED) / sizeof(FAN_SPEED[0]), 38);
      
    }
    else if (digitalRead(WIO_KEY_C) == LOW) {
      Serial.println("C Key pressed -- Rotate");
      analogWrite(WIO_BUZZER, 128);
          mySender.send(FAN_ROTATE, sizeof(FAN_ROTATE) / sizeof(FAN_ROTATE[0]), 38);
    }
    analogWrite(WIO_BUZZER, 0);
    
    spr.pushSprite(0,0); //Push to LCD
    delay(1500);
}
