#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <mutex>

#include <libwebsockets.h>

#include "WebSocketWriter.hpp"
#include "AppWebSocketWriter.hpp"

using namespace std;

namespace raspserver {

AppWebSocketWriter::AppWebSocketWriter(struct lws *wsi) : wsi(wsi){
}

AppWebSocketWriter::~AppWebSocketWriter(){
}

int AppWebSocketWriter::write(string& ss){
	lock_guard<mutex> lock(mtx);
	int i = lws_write(wsi, (unsigned char*)ss.c_str(), ss.length(), LWS_WRITE_TEXT);
	return i;
}

}

