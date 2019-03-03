#include <FastLED.h>

#define LEDPIN = 7;

//String inputString = "";
bool stringComplete = false;
String commandString = "";

float NUM_LEDS = 0;
float NUM_TIMES = 0;
float DIST = 0;
float INTERVAL = 0.0;
float REST = 0;
bool start = false;
float startMillis;
float currentMillis;
float period = 0.0;
boolean isConnected = false;

int dot = 0;

CRGB leds[750];

void setup() {
 Serial.begin(9600);
 FastLED.addLeds<WS2812B, 7>(leds,750);
 startMillis = millis();
 //Delete Later
 /*INTERVAL = 20;
 NUM_LEDS = 150;
 DIST = 1;
 period = 2*(INTERVAL*1000)/(NUM_LEDS*DIST);*/
}

void loop() {
   if(stringComplete){
    stringComplete = false;
    getCommand();
    commandString = "";
    //Serial.print(commandString);
   }
  if(start) { //Start
    currentMillis = millis();
    if((currentMillis - startMillis) >= period && dot < NUM_LEDS - 9) {
        leds[dot] = CRGB::Green;
        leds[dot+1] = CRGB::Green;
        leds[dot+2] = CRGB::Green;
        leds[dot+3] = CRGB::Green;
        leds[dot+4] = CRGB::Green;
        leds[dot+5] = CRGB::Green;
        leds[dot+6] = CRGB::Green;
        leds[dot+7] = CRGB::Green;
        leds[dot+8] = CRGB::Green;
        leds[dot+9] = CRGB::Green;
        FastLED.show();
        leds[dot] = CRGB::Black;
        leds[dot+1] = CRGB::Black;
        leds[dot+2] = CRGB::Black;
        leds[dot+3] = CRGB::Black;
        leds[dot+4] = CRGB::Black;
        leds[dot+5] = CRGB::Black;
        leds[dot+6] = CRGB::Black;
        leds[dot+7] = CRGB::Black;
        leds[dot+8] = CRGB::Black;
        leds[dot+9] = CRGB::Black;
        dot = dot + 2;
        startMillis = currentMillis;
    }

   /* else if(currentMillis - startMillis >= period && dot >= 0 && i%2 == 1 && i < DIST && k < NUM_TIMES){
        leds[dot] = CRGB::Green;
        leds[dot-1] = CRGB::Green;
        leds[dot-2] = CRGB::Green;
        leds[dot-3] = CRGB::Green;
        leds[dot-4] = CRGB::Green;
        leds[dot-5] = CRGB::Green;
        leds[dot-6] = CRGB::Green;
        leds[dot-7] = CRGB::Green;
        leds[dot-8] = CRGB::Green;
        leds[dot-9] = CRGB::Green;
        FastLED.show();
        leds[dot] = CRGB::Black;
        leds[dot-1] = CRGB::Black;
        leds[dot-2] = CRGB::Black;
        leds[dot-3] = CRGB::Black;
        leds[dot-4] = CRGB::Black;
        leds[dot-5] = CRGB::Black;
        leds[dot-6] = CRGB::Black;
        leds[dot-7] = CRGB::Black;
        leds[dot-8] = CRGB::Black;
        leds[dot-9] = CRGB::Black;
        dot--;
        startMillis = currentMillis;
    }*/
    if(dot >= NUM_LEDS - 9) {
      dot = 0;
      start = false;
    }
   }
  }

void getCommand() {
if(commandString.length() > 0){
//int cmdlength=23;
//Gets the number of LEDs to trace
if(commandString[0]=='0')
  //NUM_LEDS=686;
  NUM_LEDS=150;
else if(commandString[0]=='1')
  NUM_LEDS=750;

//Gets the number of times to repeat set
int a= my_atoi(commandString[1]);
int b= my_atoi(commandString[2]);
NUM_TIMES=(float)(a*10+b);

//Gets the "goal time" interval in seconds
int m1=my_atoi(commandString[3]);
int m2=my_atoi(commandString[4]);
int s1=my_atoi(commandString[6]);
int s2=my_atoi(commandString[7]);
int ms1=my_atoi(commandString[9]);
int ms2=my_atoi(commandString[10]);

//m1 = tens of minutes
//m2 = single minutes
//s1 = tens of seconds
//s2 = single seconds
//ms1 = tenths of seconds
//ms2 = hundredths of seconds
//((m1 * 10 + m2) * 60) + (s1 * 10) + s2 = pure seconds
//(ms1 * 10 + ms2) = pure milliseconds

INTERVAL = (float)(((m1 * 10 + m2) * 60) + (s1 * 10) + s2);
INTERVAL += (float)((ms1 * 10 + ms2)/100);

//INTERVAL=(m1*600)+(m2*60)+(s1*10)+s2+(ms1*.01)+(ms2*.001);

//Gets the "rest time" interval in seconds
m1=my_atoi(commandString[11]);
m2=my_atoi(commandString[12]);
s1=my_atoi(commandString[14]);
s2=my_atoi(commandString[15]);
ms1=my_atoi(commandString[17]);
ms2=my_atoi(commandString[18]);

REST=1.0*((m1*600)+(m2*60)+(s1*10)+s2+(ms1*.1)+(ms2*.01));

//Gets the distance to travel
int d1=my_atoi(commandString[19]);
int d2=my_atoi(commandString[20]);
int d3=my_atoi(commandString[21]);
int d4=my_atoi(commandString[22]);

DIST=(float)((((d1*1000)+(d2*100)+(d3*10)+d4))/25);

period = 2*(INTERVAL*1000)/(NUM_LEDS*DIST);


start = true;
}
}

int my_atoi(char val){
  switch(val){
    case '0':
    return 0;
    case '1':
    return 1;
    case '2':
    return 2;
    case '3':
    return 3;
    case '4':
    return 4;
    case '5':
    return 5;
    case '6':
    return 6;
    case '7':
    return 7;
    case '8':
    return 8;
    case '9':
    return 9;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    commandString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
