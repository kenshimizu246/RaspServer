#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <wiringPi.h>

#include "DS18B20.hpp"

using namespace std;

namespace raspserver {

DS18B20::DS18B20(std::string path, std::string address):path(path), address(address){
}

DS18B20::~DS18B20(){
}

float DS18B20::getTemperature()
{
	char *temp;
	float value;
	std::ifstream ifs("/sys/bus/w1/devices/28-030c9794221e/w1_slave");

	char str[buf_size];
	if (ifs.fail()) {
		std::cerr << "Failed to open file." << std::endl;
		return -1;
	}
	while (ifs.getline(str, buf_size)) {
		std::cout << "#" << str << std::endl;
		temp = strchr(str,'t');
		if(temp == NULL){
			continue;
		}
		std::cout << "temp:" << temp << std::endl;
		sscanf(temp, "t=%s",temp);
		value = atof(temp)/1000;
		std::cout << "Temp:" << value << std::endl;
		return value;
	}
  return 0;
}

}
