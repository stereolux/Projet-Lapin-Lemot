#include <SX1272.h>
#include <SPI.h>
#include <Somo2.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// Somo2 conf
Somo2 somo(8, 9);
// LoRa conf
int SENSOR_ID = 1;
int TAG_ID = 1;
char msg [30];
// NFC conf
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup() {
	Serial.begin(9600);
	setupSomo();
	setupLoRa();
	setupNfc();
}

void loop() {
	if (nfc.tagPresent()) {
		// get tag id
		NfcTag tag = nfc.read();
		String uid = tag.getUidString();
		sprintf(msg,"%i;%s#",SENSOR_ID, uid.c_str());
		Serial.println(msg);
		somo.playFromSD();
		somo.playTrack(1, 1);
		delay(2000);
		somo.sleep();
		// send tag id through LoRa
		sx1272.sendPacketTimeout(1, msg);
	}
	delay(50);
}


void setupLoRa() {
	// power on module
	sx1272.ON();
	// mode 1 is CR:CR_5;BW:BW_125;SF:SF_12: max range, slow data rate
	// see: https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/
	sx1272.setMode(4);
	// set channel to 12 and frequency to 868 MHz
	sx1272.setChannel(CH_12_868);
	// output power set to MAX
	sx1272.setPower('H');
	// set current node address to 2
	sx1272.setNodeAddress(2);
}

void setupSomo() {
	somo.begin();
	somo.reset();
	delay(1000);
	somo.setVolume(20);
	// play a signal to tell the module is operationnal
	somo.playTrack(1, 1);
	delay(2000);
	// put in sleep mode
	somo.sleep();
}

void setupNfc() {
	// false flag to turn off verbosity
	nfc.begin();
}
