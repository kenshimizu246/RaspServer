
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "PCA9685.hpp"

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LEDALL_ON_L 0xFA

#define PIN_ALL 16


using namespace std;
using namespace rapidjson;

namespace raspserver {

PCA9685::PCA9685() {
	initialized = false;
	wiringPiSetupGpio();
}

PCA9685::~PCA9685() {
}

void PCA9685::Setup(const int pinBase, const int i2cAddress/* = 0x40*/, float freq/* = 50*/){
	// Check i2c address
	fd = wiringPiI2CSetup(i2cAddress);
	if (fd < 0)
		throw runtime_error("wiringPiI2CSetup error!");

	// Setup the chip. Enable auto-increment of registers.
	int settings = wiringPiI2CReadReg8(fd, PCA9685_MODE1) & 0x7F;
	int autoInc = settings | 0x20;

	wiringPiI2CWriteReg8(fd, PCA9685_MODE1, autoInc);

	// Set frequency of PWM signals. Also ends sleep mode and starts PWM output.
	if (freq > 0)
		PWMFreq(freq);

	initialized = true;
}

void PCA9685::PWMFreq(float freq){
	// Cap at min 40 and max 1000
	freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

	// http://www.nxp.com/documents/data_sheet/PCA9685.pdf Page 24
	int prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

	int settings = wiringPiI2CReadReg8(fd, PCA9685_MODE1) & 0x7F;
	int sleep       = settings | 0x10;
	int wake	= settings & 0xEF;
	int restart = wake | 0x80;

	wiringPiI2CWriteReg8(fd, PCA9685_MODE1, sleep);
	wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE, prescale);
	wiringPiI2CWriteReg8(fd, PCA9685_MODE1, wake);

	delay(1);
	wiringPiI2CWriteReg8(fd, PCA9685_MODE1, restart);
}

void PCA9685::PWMReset(){
	wiringPiI2CWriteReg16(fd, LEDALL_ON_L    , 0x0);
	wiringPiI2CWriteReg16(fd, LEDALL_ON_L + 2, 0x1000);
}

void PCA9685::PWMWrite(int pin, int on, int off){
	PWMWrite(fd, pin, on, off);
}

void PCA9685::PWMWrite(int fd, int pin, int on, int off){
	int reg = baseReg(pin);

	wiringPiI2CWriteReg16(fd, reg    , on  & 0x0FFF);
	wiringPiI2CWriteReg16(fd, reg + 2, off & 0x0FFF);
}

void PCA9685::PWMRead(int pin, int *on, int *off){
	PWMRead(fd, pin, on, off);
}

void PCA9685::PWMRead(int fd, int pin, int *on, int *off){
	int reg = baseReg(pin);

	if (on)
		*on  = wiringPiI2CReadReg16(fd, reg);
	if (off)
		*off = wiringPiI2CReadReg16(fd, reg + 2);
}

void PCA9685::FullOn(int pin, int tf){
	FullOn(fd, pin, tf);
}

void PCA9685::FullOn(int fd, int pin, int tf){
	int reg = baseReg(pin) + 1;	     // LEDX_ON_H
	int state = wiringPiI2CReadReg8(fd, reg);

	// Set bit 4 to 1 or 0 accordingly
	state = tf ? (state | 0x10) : (state & 0xEF);

	wiringPiI2CWriteReg8(fd, reg, state);

	// For simplicity, we set full-off to 0 because it has priority over full-on
	if (tf)
		FullOff(fd, pin, 0);
}

void PCA9685::FullOff(int pin, int tf){
	FullOff(fd, pin, tf);
}

void PCA9685::FullOff(int fd, int pin, int tf){
	int reg = baseReg(pin) + 3;	     // LEDX_OFF_H
	int state = wiringPiI2CReadReg8(fd, reg);

	// Set bit 4 to 1 or 0 accordingly
	state = tf ? (state | 0x10) : (state & 0xEF);

	wiringPiI2CWriteReg8(fd, reg, state);
}

bool PCA9685::isInitialized() {
	return initialized;
}

int PCA9685::baseReg(int pin)
{
	return (pin >= PIN_ALL ? LEDALL_ON_L : LED0_ON_L + 4 * pin);
}


}

