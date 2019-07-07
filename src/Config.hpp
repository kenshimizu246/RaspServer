
#include <string>
#include <exception>
#include <stdexcept>

#include "SessionManager.hpp"

using namespace std;

#ifndef _Config_hpp
#define _Config_hpp

namespace raspserver {

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
		void load(const char* filename);
		string getAppDir();
		string getLogDir();
		string getPidFile();
		int getPort();
		bool isInitialized();

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
