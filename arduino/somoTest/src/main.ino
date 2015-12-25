#include <Somo2.h>

int Rx_pin = 9; // to Somo Tx
int Tx_pin = 8; // to Somo Rx
int i = 1;
Somo2 somo(Rx_pin,Tx_pin);

void setup() {
	somo.begin();
	somo.setVolume(20);
}

void loop() {
	somo.playTrack(1, i);
	delay(10000);
	somo.sleep();
	delay(10000);
	somo.playFromSD();
	delay(10000);
}
