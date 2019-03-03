#include <FastLED.h>

#define LEDPIN = 7;

//String inputString = "";
bool stringComplete = false;
String commandString = "";

float NUM_LEDS = 0;
float reps = 0;
float DIST = 0;
//float totalLaps = 0;
float INTERVAL = 0.0;
float REST = 0;
bool start = false;
float startMillis;
float currentMillis;
float period = 0.0;
//int num_times = 0;
boolean isConnected = false;
CRGB startColor = CRGB::Blue;
CRGB raceColor = CRGB::Green;

int lapCount = 0;

int dot = 0;

CRGB leds[750];

void setup() {
 Serial.begin(9600);
 FastLED.addLeds<WS2812B, 7>(leds,750);
 startMillis = millis();
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
    if (lapCount < DIST && !(lapCount % 2) && (currentMillis - startMillis) >= period && dot < (NUM_LEDS - 9)) {
        leds[dot] = raceColor;
        leds[dot+1] = raceColor;
        leds[dot+2] = raceColor;
        leds[dot+3] = raceColor;
        leds[dot+4] = raceColor;
        leds[dot+5] = raceColor;
        leds[dot+6] = raceColor;
        leds[dot+7] = raceColor;
        leds[dot+8] = raceColor;
        leds[dot+9] = raceColor;
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
    else if (lapCount < DIST && (lapCount % 2) && (currentMillis - startMillis) >= period && dot >= 10){
        leds[dot] = raceColor;
        leds[dot-1] = raceColor;
        leds[dot-2] = raceColor;
        leds[dot-3] = raceColor;
        leds[dot-4] = raceColor;
        leds[dot-5] = raceColor;
        leds[dot-6] = raceColor;
        leds[dot-7] = raceColor;
        leds[dot-8] = raceColor;
        leds[dot-9] = raceColor;
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
        dot = dot - 2;
        startMillis = currentMillis;
    }

    if (!(lapCount % 2) && dot >= (NUM_LEDS - 9))
        {
            lapCount++;
        }
        else if ((lapCount % 2) && dot < 10)
        {
            lapCount++;
        }
    if (lapCount >= DIST){
      start = false;
      dot = 0;
      lapCount=0;
    }
    /*else if ((int)lapCount % ((int)(num_times-1)) == 0){
      num_times+= reps;
      delay(REST*1000);
      starter();
    }*/

        }    
   }

void getCommand() {
if(commandString.length() > 0){
//int cmdlength=23;
//Gets the number of LEDs to trace
if(commandString[0]=='0')
  NUM_LEDS=686;
  //NUM_LEDS=150;
else if(commandString[0]=='1')
  NUM_LEDS=750;

//Gets the number of times to repeat set
int a= my_atoi(commandString[1]);
int b= my_atoi(commandString[2]);
reps=(float)(a*10+b);

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

REST = (float)(((m1 * 10 + m2) * 60) + (s1 * 10) + s2);
REST += (float)((ms1 * 10 + ms2)/100);
REST -= INTERVAL;

//Gets the distance to travel
int d1=my_atoi(commandString[19]);
int d2=my_atoi(commandString[20]);
int d3=my_atoi(commandString[21]);
int d4=my_atoi(commandString[22]);

DIST=(float)((((d1*1000)+(d2*100)+(d3*10)+d4))/25);

//totalLaps=DIST*reps;
//num_times = DIST;

period = 2*(INTERVAL*1000)/(NUM_LEDS*DIST);

int scolor = my_atoi(commandString[23]);
int rcolor = my_atoi(commandString[24]);

selectColor(scolor,rcolor);

starter();

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

void selectColor(int scolor,int rcolor){
  switch(scolor){
    case 0:
    {
      startColor = CRGB::Green;
      break;
    }
    case 1:
    {
      startColor = CRGB::Red;
      break;
    }
    case 2:
    {
      startColor = CRGB::Blue;
      break;
    }
  }
 switch(rcolor){
    case 0:
    {
      raceColor = CRGB::Green;
      break;
    }
    case 1:
    {
      raceColor = CRGB::Red;
      break;
    }
    case 2:
    {
      raceColor = CRGB::Blue;
      break;
    }
  } 
}
void starter(){
        leds[dot] = startColor;
        leds[dot+1] = startColor;
        leds[dot+2] = startColor;
        leds[dot+3] = startColor;
        leds[dot+4] = startColor;
        leds[dot+5] = startColor;
        leds[dot+6] = startColor;
        leds[dot+7] = startColor;
        leds[dot+8] = startColor;
        leds[dot+9] = startColor;
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
        delay(500);
        FastLED.show();
        delay(500);
        leds[dot] = startColor;
        leds[dot+1] = startColor;
        leds[dot+2] = startColor;
        leds[dot+3] = startColor;
        leds[dot+4] = startColor;
        leds[dot+5] = startColor;
        leds[dot+6] = startColor;
        leds[dot+7] = startColor;
        leds[dot+8] = startColor;
        leds[dot+9] = startColor;
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
        delay(500);
        FastLED.show();
        delay(500);
        
        
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
