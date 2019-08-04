
#include <string>
#include <exception>
#include <stdexcept>

#ifndef _DS18B20_hpp
#define _DS18B20_hpp

namespace raspserver {

class DS18B20 {
	private:
		std::string path;
		std::string address;
		std::string name{"w1_slave"};
		unsigned int buf_size{1024};

	public:
		DS18B20(std::string path, std::string address);
		~DS18B20();

		float getTemperature();
};

}


#endif /*_DS18B20_hpp*/

