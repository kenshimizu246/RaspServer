
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <syslog.h>

#include <libwebsockets.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "ErrorAction.hpp"

using namespace rapidjson;

namespace raspserver {

ErrorAction::ErrorAction(string message) : message(message) {
}

ErrorAction::~ErrorAction() {
//	BOOST_LOG_TRIVIAL(info) << "ErrorAction::~ErrorAction()";
}

void ErrorAction::doAction(unique_ptr<Document> rq, string& ss){
	Document res;
	Document::AllocatorType& alloc = res.GetAllocator();
	Value hdr(kObjectType);
	Value bdy(kObjectType);
	Value type("error");

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

//	BOOST_LOG_TRIVIAL(info) << "ErrorAction::doAction()... start";

	hdr.AddMember("type", type, alloc);

	bdy.AddMember("message", StringRef(message.c_str()), alloc);

	res.SetObject()
		.AddMember("header", hdr, alloc)
		.AddMember("body", bdy, alloc);

	res.Accept(writer);

	ss.append(buffer.GetString(), buffer.GetLength());
//	BOOST_LOG_TRIVIAL(info) << "ErrorAction::doAction()... end";
}

}

