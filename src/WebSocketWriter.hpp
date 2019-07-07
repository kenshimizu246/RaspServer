#include <string>
#include <sstream>
#include <vector>
#include <mutex>

using namespace std;

#ifndef _websocket_writer_h
#define _websocket_writer_h

namespace raspserver {

class WebSocketWriter {
	public:
		virtual int write(string& ss) = 0;
};

}

#endif /* _websocket_writer_h */
