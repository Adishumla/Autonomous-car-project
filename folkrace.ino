#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>


#define MUX_ADDRESS 0x70     // i2c-adressen till multiplexern
#define NUMBER_OF_SENSORS 2
#define SERVO_PIN 9
#define ESC_PIN 5


Servo steering;
Servo ESC;
VL53L1X lidar;

uint16_t throttle;        // 0 - 89: back. 90 stillastående. 91 - 180: fram.
uint16_t steering_angle;  // 90 = rakt fram; 120 = max höger; 60 = max vänster;
bool error = false;
uint16_t distances[NUMBER_OF_SENSORS];  // array för att lagra sensorvärden
//float speed;      // Faktor för att reglera hastigheten;


// Multiplexern har 8 i2c-anslutningar, för att välja vilken som skall användas skrivs en etta till motsvarande bit 
// Exempelvis: 0b00010000 innebär att anslutning 4 används
void i2cSelect(uint8_t bus) {
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << bus);
  Wire.endTransmission();
  
 }

// Funktionen lidarSetup initerar alla ToF-sensorer
// Argumentet sensors anger hur många sensorer som är anslutna
// I for-loopen väljs först aktuell i2c-bus, därefter initeras sensorn.
 void lidarSetup(uint8_t sensors) {
  Wire.setClock(400000);  // Sätter i2c-klockan till 400kHz
  
  for (uint8_t i=0; i < sensors; i++) {
    i2cSelect((i+2));
    
    if (!lidar.init()) {
      error = true;
    }
    lidar.setDistanceMode(VL53L1X::Long);
    lidar.setMeasurementTimingBudget(20000);
    lidar.startContinuous(20);
  }
  
}

// funktionen getDistance() lagrar sensorvärden i den globala arrayen distances;
void getDistance() {
  for (uint8_t i=0; i < NUMBER_OF_SENSORS; i++) {
    i2cSelect((i+2));
    distances[i] = lidar.read();
  }
}



void setup() {
  steering.attach(SERVO_PIN);     // Initierar servo
  ESC.attach(ESC_PIN);            // Initierar ESC
  ESC.write(90);
  delay(1000);
  lidarSetup(NUMBER_OF_SENSORS);  // Initierar lidar-sensorer
}

void loop() {
  getDistance();
  steering.write(90);
  ESC.write(120);
	if (distances[0] < distances[2]) {				// distance[0] är vänster sensor, distance[2] är höger
		steering.write(120);	// 120 = max höger
	}
	if (distances[0] > distances[2]) {
		steering.write(60);	// 60 = max vänster
	}

}
