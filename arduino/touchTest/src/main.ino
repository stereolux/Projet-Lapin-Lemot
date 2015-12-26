#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

Adafruit_CAP1188 cap = Adafruit_CAP1188();

void setup() {
	Serial.begin(9600);
	setupTouchZones();
}

void loop() {
	int zone = checkZones();
	if (zone > 0) Serial.println(zone);
	delay(50);
}

void setupTouchZones() {
	cap.begin();
	// disable multitouch
	cap.writeRegister(0x2A, 0x80);
	Serial.println("Touch ok");
}

int checkZones() {
	int touched = cap.touched();
	// no touch detected
	if (touched == 0) return 0;
	int zone = 0;
	// check for touch
	for (int i = 0; i < 8; i++) {
		if (touched & (1 << i)) {
			// TODO: if a touch if found, reset timer
			zone = i + 1;
		}
	}
	return zone;
}
