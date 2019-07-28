
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

#ifndef _HCSR04_hpp
#define _HCSR04_hpp

namespace raspserver {

class HcSr04 {
	private:
		unsigned int pinTrig;
		unsigned int pinEcho;

	public:
		HcSr04(unsigned int pinTrig, unsigned int pinEcho);
		~HcSr04();

		void setUp();
		double mesure();
};

}


#endif /*_HCSR04_hpp*/

