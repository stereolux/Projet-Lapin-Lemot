#ifndef Somo2_h
#define Somo2_h

#include <Arduino.h>
#include <SoftwareSerial.h>

#define START_BYTE 0x7E
#define FEEDBACK 0x00
#define END_BYTE 0xEF

class Somo2 {
	public:
		Somo2(int rxPin, int txPin);
		void begin();
		void playTrack(int folder, int trackNum);
		void sleep();
		void playFromSD();
		void playFromUSB();
		void increaseVolume();
		void decreaseVolume();
		void setVolume(int volume);
		void setEq(int type);
		void reset();
		void play();
		void pause();
		void stop();
	private:
		SoftwareSerial* _sp;
		bool _playing;
		void _writeCommand(byte cmd, byte param1, byte param2);
};

#endif
