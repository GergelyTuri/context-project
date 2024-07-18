// Pin Definitions
int solPin1 = 6;
int solPin2 = 5;
int anymazePin = 11; // TTL input from AnyMaze to start
int ledPin = 13; // LED indicates when odor is ON
int buttonPin = 8; // Button to manually start the protocol

// Trial variables
int solDur = 5000; // Duration of odor pulse in ms
long preOdorDur = 2000; // Delay before the odor delivery in ms
long postOdorDur = 2000; // Delay after the odor delivery in ms

// Interval Durations in milliseconds
unsigned long preTrialDuration = 10000; // Example: 10 seconds
unsigned long interTrialDuration = 5000; // Example: 5 seconds
unsigned long postTrialDuration = 15000; // Example: 15 seconds

// Control Variables
boolean toStart = 0; // Control flag for the start of the session

void setup() { 
  // initialize digital pins
  pinMode(solPin1, OUTPUT);
  pinMode(solPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(anymazePin, INPUT);

  Serial.begin(9600);
  Serial.println("Program Ready");
  Serial.println("(waiting for start signal from AnyMaze or button press)");


  // Wait for the button press to start
  while (toStart == 0) {
    checkButton();
  }
}

void loop() {
  preTrialInterval(preTrialDuration);
  // Main protocol execution
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      interTrialInterval(interTrialDuration);
    }
    activateSolenoidAndLED();
  }
  postTrialInterval(postTrialDuration);

  while (true) {
    delay(1000); // Halt further execution indefinitely after completing the cycles
  }
}

// Functions

void checkButton() {
  // Check if the button is pressed or AnyMaze sends a start signal
  if (digitalRead(buttonPin) == LOW || digitalRead(anymazePin) == HIGH) {
    toStart = true;
    unsigned long pressTime = millis();  // Capture the time of the button press
    Serial.print("Button pressed or signal received at ms: ");
    Serial.println(pressTime);
    Serial.println("Starting the protocol...");
  }
}

void activateSolenoidAndLED() {
  unsigned long eventTime = millis();

  // Pre-odor delay
  delay(preOdorDur);
  Serial.print("Pre-odor delay ends at ms: ");
  Serial.println(millis());

  // Turn on solenoid and LED
  digitalWrite(solPin1, HIGH);
  digitalWrite(ledPin, HIGH);
  eventTime = millis();
  Serial.print("Solenoid and LED ON at ms: ");
  Serial.println(eventTime);

  delay(solDur); // Odor duration

  // Turn off solenoid and LED
  digitalWrite(solPin1, LOW);
  digitalWrite(ledPin, LOW);
  eventTime = millis();
  Serial.print("Solenoid and LED OFF at ms: ");
  Serial.println(eventTime);

  // Post-odor delay
  delay(postOdorDur);
  Serial.print("Post-odor delay ends at ms: ");
  Serial.println(millis());
}

void preTrialInterval(unsigned long duration) {
  unsigned long startTime = millis();
  Serial.print("Starting pre-trial interval at ms: ");
  Serial.println(startTime);
  delay(duration);
  unsigned long endTime = millis();
  Serial.print("Pre-trial interval complete at ms: ");
  Serial.println(endTime);
}

void interTrialInterval(unsigned long duration) {
  unsigned long startTime = millis();
  Serial.print("Starting inter-trial interval at ms: ");
  Serial.println(startTime);
  delay(duration);
  unsigned long endTime = millis();
  Serial.print("Inter-trial interval complete at ms: ");
  Serial.println(endTime);
}

void postTrialInterval(unsigned long duration) {
  unsigned long startTime = millis();
  Serial.print("Starting post-trial interval at ms: ");
  Serial.println(startTime);
  delay(duration);
  unsigned long endTime = millis();
  Serial.print("Post-trial interval complete at ms: ");
  Serial.println(endTime);
}
