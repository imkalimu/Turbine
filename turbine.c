#define HALL_SENSOR_PIN 2    // Hall-effect sensor connected to digital pin 2
#define VOLTAGE_SENSOR A0    // Voltage sensor connected to analog pin A0
#define CURRENT_SENSOR A1    // Current sensor connected to analog pin A1

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
}

void loop() {
    unsigned long currentTime = millis();
    
    // Calculate RPM every second
    if (currentTime - lastTime >= 1000) {
        int rpm = (pulseCount * 60); // Convert pulses to RPM
        pulseCount = 0;              // Reset counter
        lastTime = currentTime;

        // Read voltage and current sensor values
        voltage = analogRead(VOLTAGE_SENSOR) * (5.0 / 1023.0) * 10; // Adjust scale as needed
        current = analogRead(CURRENT_SENSOR) * (5.0 / 1023.0) / 0.185; // Adjust scale for ACS712

        // Calculate power (P = V * I)
        power = voltage * current;

        // Print results to Serial Monitor
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
