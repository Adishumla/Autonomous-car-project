#include <Wire.h>
#include <VL53L1X.h>

 #define MUX_ADDRESS 0x70     // i2c-adressen till multiplexern
 #define NUMBER_OF_SENSORS 5

VL53L1X sensor;


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
    i2cSelect(i);
    
    if (!sensor.init()) {
      Serial.print("Failed to initalise sensor");
      Serial.println(i);
    
    }
    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(20000);
    sensor.startContinuous(20);
  }
  
}

void setup() {
  Wire.begin();               // startar i2c
  Serial.begin(115200);  
  Serial.println("\nStarting..");

  lidarSetup(NUMBER_OF_SENSORS);  // Initierar sensorer
}



void loop() {
  getDistance();
}


// Loopen väljer först aktuell sensor (i2c-bus) och skriver sedan ut värdet från sensor.read()
void getDistance() {
  for (uint8_t i=0; i < NUMBER_OF_SENSORS; i++) {
    i2cSelect(i);
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(":\t");
    Serial.print(sensor.read());
    Serial.print("\t\t");
  }
  Serial.println("\n");
}
