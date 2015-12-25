#include <Somo2.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

Somo2::Somo2(int rxPin, int txPin) {
	_sp = new SoftwareSerial(rxPin, txPin);
	_playing = false;
}

void Somo2::begin() {
	_sp->begin(9600);
}

void Somo2::_writeCommand(byte cmd, byte param1, byte param2) {
	// calculate checksum
	word checksum = 0xFFFF - ((word)cmd + (word)FEEDBACK + (word)param1 + (word)param2) + 1;
	// upper 8 bits
	byte checksum1 = (byte)(checksum >> 8);
	// lower 8 bits
	byte checksum2 = (byte)checksum;
	// bytes to write
	byte bytes[] = {START_BYTE, cmd, FEEDBACK, param1, param2, checksum1, checksum2, END_BYTE};
	_sp->write(bytes, sizeof(bytes));
	delay(50);
}

void Somo2::playTrack(int folder, int trackNum) {
	_playing = true;
	_writeCommand(0x0F, (byte)folder, (byte)trackNum);
}

void Somo2::sleep() {
	_writeCommand(0x0A, 0x00, 0x00);
}

void Somo2::playFromSD() {
	_writeCommand(0x09, 0x00, 0x02);
}

void Somo2::playFromUSB() {
	_writeCommand(0x09, 0x00, 0x01);
}

void Somo2::increaseVolume() {
	_writeCommand(0x04, 0x00, 0x00);
}

void Somo2::decreaseVolume() {
	_writeCommand(0x05, 0x00, 0x00);
}

void Somo2::setVolume(int volume) {
	if (volume > 30) volume = 30;
	if (volume < 0) volume = 0;
	_writeCommand(0x06, 0x00, (byte)volume);
}

void Somo2::setEq(int type) {
	_writeCommand(0x07, 0x00, (byte)type);
}

void Somo2::reset() {
	_writeCommand(0x0C, 0x00, 0x00);
}

void Somo2::play() {
	if (!_playing) {
		_playing = true;
		_writeCommand(0x0D, 0x00, 0x00);
	}
}

void Somo2::pause() {
	if (_playing) {
		_playing = false;
		_writeCommand(0x0E, 0x00, 0x00);
	}
}

void Somo2::stop() {
	_playing = false;
	_writeCommand(0x16, 0x00, 0x00);
}
