/*
 * Clay Lacefield 2022
 * Arduino script to activate pneumatic solenoid valve for predator odor
 * delivery.
 * Starts on input from AnyMaze I/O or button press.
 * Delivers odor at specific latency, for specific duration.
 * Periodic sync output for video/photometry/ephys/imaging synchronization.
 * LED lights during odor delivery.
 * 7/18/2024 GT's mods.
 */

////////////////////
// Session variables
long solTime = 2000; //0 time in ms of odor delivery start 
int solDur = 8000; //0 duration of odor pulse
long postOdor = 3000; //0 pause after odor delivery
////////////////////

// pins
int solPin1 = 6;
int solPin2 = 5;
int anymazePin = 11; // TTL input from AnyMaze to start
int ledPin = 13; // currently, LED when odor is ON
int buttonPin = 8;
int syncPin = 12; // pin for video sync

// sync output signal params
int syncDur = 500;
long syncStartTime = 0;
long syncIntv = 1000; //0
long lastSyncTime = 0;
int prevSync = 0;

// odor delivery parameters
long solStartTime = 0;
int prevSol = 0;
int solOn = 0;

// session variables
long startTime = 0;
int toStart = 0;
int loopCount = 0; // Counter for the number of loops

////////SETUP
void setup() {
  // initialize digital pins
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(syncPin, OUTPUT);
  pinMode(anymazePin, INPUT);

  Serial.begin(9600);
  Serial.println("Program Ready");
  Serial.println("(waiting for start signal from AnyMaze or button press)");


  // Wait for the button press to start
  while (toStart == 0) {
    checkButton();
}
}
///////LOOP
void loop() {
  if (loopCount < 6) {
    Serial.print("Loop number: ");
    Serial.println(loopCount + 1);
    checkSol(); // check solenoid state
    checkSyncState(); // check sync pulse
    delay(postOdor);
    loopCount++; // Increment loop count after completing a cycle
  }    
    else{
      Serial.println("Session complete");
      while(1){
        delay(1000); // do nothing}
  }
}
}
////////SUBFUNCTIONS

//////////////////////////////////
void checkSol() { // check the solenoid valve state (to turn on/off)
  if (prevSol==0 && millis() - startTime >= solTime) {
    digitalWrite(solPin1, HIGH);
    digitalWrite(ledPin, HIGH); // light LED during odor delivery
    solStartTime = millis();
    Serial.print("odorON, ms= ");
    Serial.println(solStartTime);
    solOn = 1;
    prevSol = 1;
  } 
  else if (solOn == 1 && millis() - solStartTime >= solDur) {
    digitalWrite(solPin1, LOW);
    digitalWrite(ledPin, LOW); 
    Serial.print("odorOff, ms= ");
    Serial.println(millis());
    solOn = 0;
  }
}
/////////////////////////////
void checkButton() { // press button to start program (or trigger from AnyMaze)
  if (digitalRead(buttonPin) == LOW || digitalRead(anymazePin) == HIGH) {
    startTime = millis();
    toStart = 1;
    prevSol = 0; // reset solenoid event
    Serial.print("START, ms=");
    Serial.println(startTime);
  }
}
//////////////////////////////////
void checkSyncState() {
  if (prevSync == 1 && millis() - syncStartTime >= syncDur) {
    digitalWrite(syncPin, LOW);
    prevSync = 0;
  }
  else if (millis() - lastSyncTime >= syncIntv) {
    digitalWrite(syncPin, HIGH);
    syncStartTime = millis();
    Serial.print("syncOut, ms= ");
    Serial.println(syncStartTime);
    lastSyncTime = syncStartTime;
    prevSync = 1;
  }
}
