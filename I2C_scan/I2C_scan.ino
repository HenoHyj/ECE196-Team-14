#include <Wire.h>

#define SDA_PIN 35          // I2C SDA pin
#define SCL_PIN 36          // I2C SCL pin
#define INTERRUPT_PIN 37    // Interrupt pin

volatile bool interruptTriggered = false; // Flag for interrupt status

void IRAM_ATTR handleInterrupt() {
  interruptTriggered = true; // Set the flag when the interrupt is triggered
}

void setup() {
  Serial.begin(115200); // Start the Serial Monitor
  while (!Serial);      // Wait for Serial Monitor to open (optional)

  // Initialize I2C with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("\nI2C Scanner Starting...");

  // Configure interrupt pin
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);  // Set as input with pull-up resistor
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handleInterrupt, FALLING); // Trigger on falling edge
}

void loop() {
  // Check for I2C devices
  Serial.println("Scanning for I2C devices...");
  
  int devicesFound = 0; // Counter for I2C devices

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address); // Initiate transmission with device
    uint8_t error = Wire.endTransmission(); // End transmission and check for errors

    if (error == 0) {
      // Device found at this address
      Serial.print("I2C device found at address: 0x");
      if (address < 16) Serial.print("0"); // Leading zero for single hex digit
      Serial.println(address, HEX);
      devicesFound++;
    } else if (error == 4) {
      // Unknown error
      Serial.print("Error at address: 0x");
      if (address < 16) Serial.print("0"); // Leading zero for single hex digit
      Serial.println(address, HEX);
    }
  }

  if (devicesFound == 0) {
    Serial.println("No I2C devices found.\n");
  } else {
    Serial.println("I2C scan complete.\n");
  }

  // Check if interrupt was triggered
  if (interruptTriggered) {
    interruptTriggered = false; // Reset the flag
    Serial.println("Interrupt detected on pin 37!");
  }

  delay(5000); // Wait 5 seconds before scanning again
}
