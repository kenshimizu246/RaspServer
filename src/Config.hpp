
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
		void load(const char* filename);
		string getAppDir();
		string getLogDir();
		string getPidFile();
		int getPort();
		bool isInitialized();
		shared_ptr<I2CConf> getI2CConf(int i);
		unsigned int getI2CSize();

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
