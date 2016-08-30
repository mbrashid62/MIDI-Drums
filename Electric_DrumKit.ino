#include <MIDI.h> //MIDI library

MIDI_CREATE_DEFAULT_INSTANCE();

#define KICKTHRESHOLD  150
#define SNARETHRESHOLD 150
#define TOMTHRESHOLD 100
#define HIHATTHRESHOLD  100
#define CRASHTHRESHOLD  150

//variables for drum intervals 
unsigned long snareInterval = 20;
unsigned long previousSnareMillis = 0;
unsigned long kickInterval = 20;
unsigned long previousKickMillis = 0;
unsigned long tomInterval = 20;
unsigned long previousTomMillis = 0;
unsigned long hihatInterval = 20;
unsigned long previousHihatMillis = 0;
unsigned long crashInterval = 10;
unsigned long previousCrashMillis = 0;

//initializing pins
int kickPin  = A0;
int snarePin = A1;
int tomPin = A2;
int hihatPin = A3;
int crashPin = A4;

//sensor values
int kickValue  = 0;
int snareValue = 0;
int tomValue = 0;
int hihatValue = 0;
int crashValue = 0;


// maping to MIDI bytes through GarageBand (hardcoded values for each note)
byte KICK = 36;
byte SNARE = 38;
byte TOM = 71;
byte HIHAT = 42;
byte CRASH = 49;

//begin
void setup() {
  MIDI.begin();
  Serial.begin(115200);
}

boolean noteReady(unsigned long previousMillis, unsigned long interval) {
  
  if((unsigned long)(millis() - previousMillis) >= interval) {
    return true;
  }
  else {
    return false;
  }
}

void hitNote(int value, byte note, int multiply, int treshold) {
  int velocity = value - treshold;
  //velocity = 127; //velocity is for note sensitivity
  
  if(velocity > 127)
    velocity = 127;
  else if(velocity < 0)
    velocity = 0;
  
  MIDI.sendNoteOn(note, velocity, 1);
  MIDI.sendNoteOff(note, 0, 1);
}

void loop() {

  //read in values from sesnors
  kickValue  = analogRead(kickPin);
  snareValue = analogRead(snarePin);
  tomValue = analogRead(tomPin);
  hihatValue = analogRead(hihatPin);
  crashValue = analogRead(crashPin);

  //kick
  if(kickValue >= KICKTHRESHOLD && noteReady(previousKickMillis, kickInterval)) {
    previousKickMillis = millis();   
    hitNote(kickValue, KICK, 5, KICKTHRESHOLD);
  }

  //snare
  if(snareValue >= SNARETHRESHOLD && noteReady(previousSnareMillis, snareInterval)) {
    previousSnareMillis = millis();
    hitNote(snareValue, SNARE, 1, SNARETHRESHOLD);
  }

  //tom 
  if(tomValue >= TOMTHRESHOLD && noteReady(previousHihatMillis, tomInterval)) {
    previousTomMillis = millis();
    hitNote(tomValue, TOM, 1, TOMTHRESHOLD);

  }

  //hihat
  if(hihatValue >= HIHATTHRESHOLD && noteReady(previousHihatMillis, hihatInterval)) {
    previousHihatMillis = millis();
    hitNote(hihatValue, HIHAT, 1, HIHATTHRESHOLD);
  }

  //crash
  if(crashValue >= CRASHTHRESHOLD && noteReady(previousCrashMillis, crashInterval)) {
    previousCrashMillis = millis();
    hitNote(crashValue, CRASH, 1, CRASHTHRESHOLD);
  }
}
