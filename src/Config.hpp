
#include <string>
#include <exception>
#include <stdexcept>

#include "SessionManager.hpp"

using namespace std;

#ifndef _Config_hpp
#define _Config_hpp

namespace raspserver {

class I2CConf {
	public:
		unsigned int address;
		unsigned int hertz;
		I2CConf(unsigned int address, unsigned int hertz): address(address), hertz(hertz) {
		}
};

class HcSr04Conf{
	public:
		unsigned int pinTrig;
		unsigned int pinEcho;
		HcSr04Conf(unsigned int pinTrig,unsigned int pinEcho):pinTrig(pinTrig),pinEcho(pinEcho) {
		}
};

class Config {
	private:
		Config();
		~Config();
		bool initialized;
		string appdir;
		string logdir;
		string pidfile;
		int port;
		bool daemon;
		SessionManager *sessionManager;

	public:
		vector<shared_ptr<I2CConf>> i2cs;
		vector<shared_ptr<HcSr04Conf>> hcsr04s;
		void load(const char* filename);
		string getAppDir();
		string getLogDir();
		string getPidFile();
		int getPort();
		bool isInitialized();
		shared_ptr<I2CConf> getI2CConf(int i);
		unsigned int getI2CSize();
		shared_ptr<HcSr04Conf> getHcSr04Conf(int i);
		unsigned int getHcSr04ConfSize();

		bool isDaemon();

		void setSessionManager(SessionManager* sm);
		SessionManager* getSessionManager();

		static Config& getInstance() {
			static Config config;
			return config;
		}
};

}

#endif /*_Config_hpp*/
