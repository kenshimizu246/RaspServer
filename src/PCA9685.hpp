
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
		bool initialized;
		int fd;

	public:
		bool isInitialized();

		void wiringPiSetupGpio();
		void pca9685Setup(const int pinBase, const int i2cAddress/* = 0x40*/, float freq/* = 50*/);
		void pca9685PWMFreq(float freq);
		void pca9685PWMReset();
		void pca9685PWMWrite( int pin, int on, int off);
		void pca9685PWMRead(int pin, int *on, int *off);

		void pca9685FullOn(int pin, int tf);
		void pca9685FullOff(int pin, int tf);

		static PCA9685& getInstance() {
			static PCA9685 pca9685;
			return pca9685;
		}
};

}

#endif /*_PCA9685_hpp*/
