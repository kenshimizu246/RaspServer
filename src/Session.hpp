#include <sstream>
#include <time.h>
#include <memory>

#include "WebSocketWriter.hpp"

using namespace std;

#ifndef _session_h
#define _session_h

namespace raspserver {

class Session {
	public:
		Session(string id, bool valid);
		Session(string id, bool valid, time_t start_time);
		~Session();
		void update();

		string id;
		bool valid;
		time_t start_time;
		time_t last_accessed;

		shared_ptr<WebSocketWriter> writer;
};

}

#endif /* _session_h */
