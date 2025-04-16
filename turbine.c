#include <Wire.h>
#include "RTClib.h"

#define HALL_SENSOR_PIN 2    // Hall-effect sensor connected to digital pin 2
#define VOLTAGE_SENSOR A0    // Voltage sensor connected to analog pin A0
#define CURRENT_SENSOR A1    // Current sensor connected to analog pin A1

RTC_DS3231 rtc;

volatile int pulseCount = 0; // Counter for rotor passes
unsigned long lastTime = 0;  // Time tracking for RPM calculation

float voltage = 0.0;
float current = 0.0;
float power = 0.0;

// Interrupt service routine for hall sensor
void countPulse() {
    pulseCount++;  
}

void setup() {
    Serial.begin(9600);              // Start serial communication
    pinMode(HALL_SENSOR_PIN, INPUT); // Set Hall sensor as input
    attachInterrupt(digitalPinToInterrupt(HALL_SENSOR_PIN), countPulse, RISING); // Interrupt on rotor pass

    // RTC initialization
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    // Uncomment to set the RTC time once, then comment it again
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
    unsigned long currentTime = millis();
    
    // Calculate RPM every second
    if (currentTime - lastTime >= 1000) {
        int rpm = (pulseCount * 60); // Convert pulses to RPM
        pulseCount = 0;              // Reset counter
        lastTime = currentTime;

        // Read voltage and current sensor values
        voltage = analogRead(VOLTAGE_SENSOR) * (5.0 / 1023.0) * 10;         // Adjust scale as needed
        current = analogRead(CURRENT_SENSOR) * (5.0 / 1023.0) / 0.185;      // Adjust scale for ACS712
        power = voltage * current;

        // Get current time
        DateTime now = rtc.now();

        // Print timestamp in one line
        Serial.print("Timestamp: ");
        Serial.print(now.year()); Serial.print("-");
        if (now.month() < 10) Serial.print("0");
        Serial.print(now.month()); Serial.print("-");
        if (now.day() < 10) Serial.print("0");
        Serial.print(now.day()); Serial.print(" ");
        if (now.hour() < 10) Serial.print("0");
        Serial.print(now.hour()); Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.print(now.minute()); Serial.print(":");
        if (now.second() < 10) Serial.print("0");
        Serial.print(now.second());
        Serial.print(" | ");

        // Print measurements
        Serial.print("RPM: ");
        Serial.print(rpm);
        Serial.print(" | Voltage: ");
        Serial.print(voltage);
        Serial.print("V | Current: ");
        Serial.print(current);
        Serial.print("A | Power: ");
        Serial.print(power);
        Serial.println("W");
    }
}
