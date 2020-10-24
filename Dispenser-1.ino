#include <NewPing.h>
#include <FastLED.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define pumpOut 3
#define LEDOUT 4
#define LEDSTRIPPIN 5
#define DURATIONPIN A0
#define DISTANCEPIN A1

int gPumpDuration = 1000;
int gNumFlashes = 3;
int gPauseTime = 2000;


//LEDStrips
#define NUM_LEDS_PER_STRIP 2
CRGB leds[NUM_LEDS_PER_STRIP];

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//================ Set Up ================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pumpOut,OUTPUT);
  pinMode(LEDOUT,OUTPUT);
  pinMode(DURATIONPIN,INPUT);
  FastLED.addLeds<NEOPIXEL, LEDSTRIPPIN>(leds, NUM_LEDS_PER_STRIP);
  digitalWrite(LEDOUT,LOW);
  digitalWrite(LEDOUT,HIGH);
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  FastLED.show();
  lightShow();
 
}
//=================Loop ===================================
void loop() {
  // put your main code here, to run repeatedly:

 delay(200);

       if ( isTarget() == true) {
          
          dispenseSanitizer();
       }
       else{
          //lightShow();
       }
}

//=============== isTarget (there) ===================
bool isTarget(){
int theRange = 0;
int theDistance = 0;
int distanceSetPoint = 0;

distanceSetPoint = map(analogRead(DISTANCEPIN),0,1024,0,MAX_DISTANCE);
theRange = sonar.ping_cm();
Serial.print("SetPoint:");Serial.print(distanceSetPoint);Serial.print(" ");
Serial.print("Range:");Serial.println(theRange);
if ( theRange > 0 && theRange < distanceSetPoint){
      return true;
}
else{    
   return false; 
}
//     if (theRange != 0 ){
//      return true;      
//     }
//     else{
//      return false;     
//     }
 
}

//========== dispenseSanitizer ===============

void dispenseSanitizer(){
bool handThere = true;
digitalWrite(LEDOUT,LOW);
digitalWrite(pumpOut,HIGH);
testLEDStrip();
digitalWrite(pumpOut,LOW);
while ( handThere == true){
  handThere = isTarget(); 
  delay(200);
  //Serial.println(handThere);
}
delay(gPauseTime);
digitalWrite(LEDOUT,HIGH);
lightShow();  
}

//============ test LED Strip =========
void testLEDStrip(){
  // Turn the LED on, then pause
//gPumpDuration
//gNumFlashes
gPumpDuration = map(analogRead(DURATIONPIN),0,1024,500,3000);
//Serial.print("PumpDuration:");Serial.println(gPumpDuration);

int lPulseDuration = gPumpDuration / (gNumFlashes + 2) ;
 for (int i = 0; i < gNumFlashes; i++){
    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;
    FastLED.show();
    delay(lPulseDuration);
    // Now turn the LED off, then pause
    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
    FastLED.show();
    delay(lPulseDuration);
 }
leds[0] = CRGB::Black;
leds[1] = CRGB::Black;
FastLED.show();
}


//============== Light show ============================

void lightShow(){
    FastLED.setBrightness( 0 );
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    FastLED.show();
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Green;
    FastLED.show();   
for (int i = 0; i < 254 ; i +=1){
     FastLED.setBrightness( i );
     FastLED.show();
     delay(7);
}
   

  
}
