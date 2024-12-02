const int NUM_POTS = 3;

int analogPins[NUM_POTS] = {A0, A1, A2};  
const int redLED = D4;          
const int greenLED = D3; 

const int password[NUM_POTS] = {3, 4, 5}; 

const int threshold = 10; 
const int stableCountThreshold = 10; 

// enum - 0,1,2
enum PotState {IDLE, MOVING, STOPPED};
PotState potStates[NUM_POTS];

int inputs[NUM_POTS]; 
int lastInputs[NUM_POTS]; 
int stableCount[NUM_POTS];

void setup() {
  Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  for (int i = 0; i < NUM_POTS; i++) {
    int val = analogRead(analogPins[i]);
    inputs[i] = map(val, 0, 10000 , 0, 9);
    lastInputs[i] = inputs[i];
    potStates[i] = IDLE;
    stableCount[i] = 0;
  }

  // Initial password check
  checkPassword();

  Serial.print("Password: ");
  for (int i = 0; i < NUM_POTS; i++) {
    Serial.print(inputs[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  bool hasStopped = false;

  for (int i = 0; i < NUM_POTS; i++) {
    int val = analogRead(analogPins[i]);
    inputs[i] = map(val, 0, 10000, 0 , 9);

    if (inputs[i] != lastInputs[i]) {

      //moving
      potStates[i] = MOVING;
      stableCount[i] = 0;

    } else {

      //is stable
      if (potStates[i] == MOVING) {
        stableCount[i]++;
        if (stableCount[i] >= stableCountThreshold) {
          // Potentiometer has stopped moving
          potStates[i] = STOPPED;
          hasStopped = true;
        }
      }
    }

    lastInputs[i] = inputs[i];

    
    // Serial.print(i);
    // Serial.println(inputs[i]);
    

  }

  if (hasStopped) {
    checkPassword();

    Serial.print("Password: ");
    for (int i = 0; i < NUM_POTS; i++) {
      Serial.print(inputs[i]);
      Serial.print(" ");
    }
    Serial.println();

    // Reset states to IDLE
    for (int i = 0; i < NUM_POTS; i++) {
      if (potStates[i] == STOPPED) {
        potStates[i] = IDLE;
      }
    }
  }

  delay(50);
}

//check password
void checkPassword() {

  bool correct = true;

  for (int i = 0; i < NUM_POTS; i++) {
    if (inputs[i] != password[i]) {
      correct = false;
      break;
    }
  }

  if (correct) {

    Serial.println("Password Correct");
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);

  } else {

    Serial.println("Password Incorrect");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    
  }
}
