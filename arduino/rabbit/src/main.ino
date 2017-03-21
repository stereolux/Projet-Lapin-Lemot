#include <SX1272.h>
#include <SPI.h>
#include <Somo2.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <Adafruit_CAP1188.h>
#include <elapsedMillis.h>

// ID of the given rabbit
#define RABBIT_ID 1

// Somo2 conf
Somo2 somo(8, 9);
// NFC conf
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
// capacitative touch conf
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// char array to handle message to be sent through LoRa
char msg [30];
// flag to know if capacitative touch zones are active
bool touchActive = false;
// check elapsed time
elapsedMillis timeElapsed;
// 30s without user action means user left
long interval = 30000;

void setup() {
	//Serial.begin(9600);
	setupSomo();
	setupLoRa();
	setupNfc();
	setupTouchZones();
	pinMode(13, OUTPUT);
	digitalWrite(13, 0);
}

void loop() {
	if (nfc.tagPresent() && !touchActive) {
		// touch can trigger sound since NFC tag is detected
		touchActive = true;
		digitalWrite(13, 1);
		// clear cap buffer so no remaining data is in it
		cap.touched();
		// user action detected, start timer
		timeElapsed = 0;
		computeMessage();
		playSound(1, 2000);
		sendLoRaMessage();
	}
	if (touchActive) {
		int zone = checkZones();
		// one zone has been touched
		if (zone > 0) {
			// get sound corresponding to the zone
			int soundID = zone + 1;
			playSound(soundID, 10000);
			// clear cap buffer so no remaining data is in it
			cap.touched();
			// reset the timer since a user action has been detected
			timeElapsed = 0;
		}
	}
	if (timeElapsed > interval) {
		touchActive = false;
		digitalWrite(13, 0);
	}
	delay(50);
}

/**
 * Check if any capacitative zone is touched and play according sound
 */
int checkZones() {
	int touched = cap.touched();
	int zone = 0;
	// touch was detected
	if (touched != 0) {
		// check for touch
		for (int i = 0; i < 8; i++) {
			if (touched & (1 << i)) {
				zone = i + 1;
			}
		}
	}
	return zone;
}

/**
 * Compute tag ID and prepare message
 */
void computeMessage() {
	// get tag id
	NfcTag tag = nfc.read();
	String uid = tag.getUidString();
	sprintf(msg,"%i;%s#",RABBIT_ID, uid.c_str());
	//Serial.println(msg);
}

/**
 * Send message through LoRa
 */
void sendLoRaMessage () {
	// send message through LoRa
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
	// somo.playFromSD();
	// play sound
	somo.playTrack(1, soundID);
	// delay(duration);
	// put the module in sleep mode to save energy
	// somo.sleep();
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
}

/**
 * Setup Somo2 module
 */
void setupSomo() {
	somo.begin();
	somo.reset();
	delay(1000);
	somo.setVolume(30);
	playSound(1, 2000);
}

/**
 * Setup NFC reader
 */
void setupNfc() {
	// false flag to turn off verbosity
	nfc.begin(true);
}

/**
 * Setup capacitative touch zones
 */
void setupTouchZones() {
	cap.begin();
	// disable multitouch
	cap.writeRegister(0x2A, 0x80);
}
