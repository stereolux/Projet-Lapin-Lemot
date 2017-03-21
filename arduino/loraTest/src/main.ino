#include <SX1272.h>
#include <SPI.h>

int SENSOR_ID = 1;
int TAG_ID = 1;

char msg [30];
int buttonPin = 8;
int previousSensorVal = HIGH;
int e;

void setup() {
	Serial.begin(9600);
	// button pin
	pinMode(buttonPin, INPUT_PULLUP);
	// power on module
	sx1272.ON();
	// mode 4 is CR:CR_5;BW:BW_500;SF:SF_12
	// see: http://www.libelium.com/development/waspmote/documentation/lora-gateway-tutorial/
	sx1272.setMode(1);
	// set channel to 12 and frequency to 868 MHz
	sx1272.setChannel(CH_12_868);
	// output power set to High
	sx1272.setPower('H');
	// set current node address to 2
	sx1272.setNodeAddress(2);
}

void loop(void) {
	// int sensorVal = digitalRead(buttonPin);
	// if (sensorVal == HIGH && previousSensorVal == LOW) {
		sprintf(msg,"%i;%i#",SENSOR_ID, TAG_ID);
		Serial.println(msg);
		e = sx1272.sendPacketTimeout(1, msg);
		Serial.print(F("Packet sent, state "));
		Serial.println(e, DEC);
	// }
	// previousSensorVal = sensorVal;
	delay(5000);
}
