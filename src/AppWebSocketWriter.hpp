#include <string>
#include <sstream>
#include <vector>
#include <mutex>

#include "WebSocketWriter.hpp"

using namespace std;

#ifndef _app_websocket_writer_h
#define _app_websocket_writer_h

namespace raspserver {

class AppWebSocketWriter : public WebSocketWriter{
	public:
		AppWebSocketWriter(struct lws *wsi);
		~AppWebSocketWriter();

		int write(string& ss);

	private:
		struct lws *wsi;
		mutex mtx;
};

}

#endif /* _app_websocket_writer_h */
