#define SENSOR0_PIN 7 // Pin för IR-sensorn
#define MOTOR0 5      // Pin 0 för motordrivare
#define MOTOR1 6      // Pin 1 för motordrivare
#define MOTORSPEED 9  // PWM-pin för motordrivare

void setup() {
  // Anger input/output för pins som används:
  pinMode(MOTOR0, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTORSPEED, OUTPUT);
  pinMode(SENSOR0_PIN, INPUT);

  // Variabeln proximity är != 0 om något är framför den och == 0 inget är framför den
  int proximity = digitalRead(SENSOR0_PIN);
}


void loop() {
  proximity = digitalRead(SENSOR0_PIN);  // Uppdaterar IR-sensorns värde

  // Om MOTOR0 = HIGH och MOTOR1 = LOW kör bilen framåt:
  digitalWrite(MOTOR0, HIGH;
  digitalWrite(MOTOR1, LOW);

  // Om IR-sensorn inte upptäcker något hinder sätts hastighet till 200 (av 0-255):
  if (proximity == 0) {
    analogWrite(MOTORSPEED, 200);
  } else {                        // Om hinder upptäcks sätts hastighet till 100 och bilen börjar backa (MOTOR0 sätts till LOW och MOTOR1 till HIGH)
    analogWrite(MOTORSPEED, 100);
    digitalWrite(MOTOR0, LOW);
    digitalWrite(MOTOR1, HIGH);
    delay(3000);                // Fördröjning gör att bilen fortsätter backa i tre sekunder, om IR-sensorn inte upptäckt något hinder då kör bilen framåt igen
  }
}
