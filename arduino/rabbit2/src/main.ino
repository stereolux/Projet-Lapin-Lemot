#include <SX1272.h>
#include <SPI.h>
#include <Somo2.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
//#include <elapsedMillis.h>

// debug flag to turn on/off serial output
#define DEBUG true
// ID of the given rabbit
int RABBIT_ID = 1;

// Somo2 conf
Somo2 somo(8, 9);
// NFC conf
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

// char array to handle message to be sent through LoRa
char msg [30];
// flag to know if capacitative touch zones are active
bool touchActive = false;
// check elapsed time
//elapsedMillis timeElapsed;
// 30s without user action means user left
//long interval = 5000;

void setup() {
	if (DEBUG) Serial.begin(9600);
	setupSomo();
	setupLoRa();
	setupNfc();
	//setupTouchZones();
}

void loop() {
	if (nfc.tagPresent()) {
		// touch can trigger sound since NFC tag is detected
		//touchActive = true;
		// user action detected, start timer
		//timeElapsed = 0;
		if (DEBUG) Serial.println("[NFC] Tag!");
		sendTagID();
		playSound(1, 2000);
	}
	/*if (touchActive) {
		checkZones();
	}
	if (timeElapsed > interval) {
		touchActive = false;
	}*/
	delay(50);
}

/**
 * Check if any capacitative zone is touched and play according sound
 */
void checkZones() {
	if (DEBUG) Serial.println("[Touch] TODO: Check zones");
	// if a touch if found, reset timer
}

/**
 * Compute NFC tag ID and send it through LoRa
 */
void sendTagID () {
	// get tag id
	NfcTag tag = nfc.read();
	String uid = tag.getUidString();
	sprintf(msg,"%i;%s#",RABBIT_ID, uid.c_str());
	if (DEBUG) {
		Serial.print("[LoRa] ");
		Serial.println(msg);
	}
	// send tag id through LoRa
	sx1272.sendPacketTimeout(1, msg);
	delay(100);
}

/**
 * Play a sound from Somo2 module
 * @param {int} soundID the id of the sound in folder "01"
 * @param {long} duration the duration of the sound
 */
void playSound(int soundID, long duration) {
	// wake up Somo2 module
	somo.playFromSD();
	// play sound
	somo.playTrack(1, soundID);
	delay(duration);
	// put the module in sleep mode to save energy
	somo.sleep();
}

/**
 * Setup LoRa communication
 */
void setupLoRa() {
	// power on module
	sx1272.ON();
	// mode 1 is CR:CR_5;BW:BW_125;SF:SF_12: max range, slow data rate
	// see: https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/
	sx1272.setMode(1);
	// set channel to 12 and frequency to 868 MHz
	sx1272.setChannel(CH_12_868);
	// output power set to MAX
	sx1272.setPower('H');
	// set current node address to 2
	sx1272.setNodeAddress(2);
	if (DEBUG) Serial.println("[LoRa] OK");
}

/**
 * Setup Somo2 module
 */
void setupSomo() {
	somo.begin();
	somo.reset();
	delay(1000);
	somo.setVolume(20);
	playSound(1, 2000);
	if (DEBUG) Serial.println("[Somo] OK");
}

/**
 * Setup NFC reader
 */
void setupNfc() {
	// false flag to turn off verbosity
	nfc.begin(DEBUG);
	if (DEBUG) Serial.println("[NFC] OK");
}

/**
 * Setup capacitative touch zones
 */
// void setupTouchZones() {
// 	if (DEBUG) Serial.println("[Touch] TODO : Setup");
// }
