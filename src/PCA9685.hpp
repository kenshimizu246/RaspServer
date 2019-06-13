
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

#ifndef _PCA9685_hpp
#define _PCA9685_hpp

namespace raspserver {

class PCA9685 {
	private:
		PCA9685();
		~PCA9685();
		static int baseReg(int pin);
		static void internalPwmWrite(struct wiringPiNodeStruct *node, int pin, int value);
		static void internalOnOffWrite(struct wiringPiNodeStruct *node, int pin, int value);
		static int internalOffRead(struct wiringPiNodeStruct *node, int pin);
		static int internalOnRead(struct wiringPiNodeStruct *node, int pin);

		static void FullOn(int fd, int pin, int tf);
		static void FullOff(int fd, int pin, int tf);
		static void PWMWrite(int fd, int pin, int on, int off);
		static void PWMRead(int fd, int pin, int *on, int *off);

		bool initialized;
		int fd;

	public:
		bool isInitialized();

		void Setup(const int pinBase, const int i2cAddress/* = 0x40*/, float freq/* = 50*/);
		void PWMFreq(float freq);
		void PWMReset();
		void PWMWrite( int pin, int on, int off);
		void PWMRead(int pin, int *on, int *off);

		void FullOn(int pin, int tf);
		void FullOff(int pin, int tf);

		static PCA9685& getInstance() {
			static PCA9685 instance;
			return instance;
		}
};

}

#endif /*_PCA9685_hpp*/
