#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup(void) {
	Serial.begin(9600);
	// false flag to turn off verbosity
	nfc.begin(false);
}


void loop(void) {
	if (nfc.tagPresent()) {
		NfcTag tag = nfc.read();
		String uid = tag.getUidString();
		Serial.println(uid);
		// 5 sec before being able to read again
		delay(5000);
	}
	delay(500);
}
