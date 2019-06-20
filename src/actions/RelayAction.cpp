
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

#include <wiringPi.h>
#include "RelayAction.hpp"

using namespace rapidjson;

namespace raspserver {

RelayAction::RelayAction() {
}

RelayAction::~RelayAction() {
}

void RelayAction::doAction(unique_ptr<Document> rq, string& ss){
  Document res;
  Document::AllocatorType& alloc = res.GetAllocator();
  Value hdr(kObjectType);
  Value bdy(kObjectType);
  Value type("null");

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  if(!(*rq)["body"].HasMember("pin")){
    throw invalid_argument("pin is mandatory!");
  }

  if(!(*rq)["body"].HasMember("on")){
    throw invalid_argument("on is mandatory!");
  }

  int pin = (*rq)["body"]["pin"].GetInt();
  int on = (*rq)["body"]["on"].GetInt();

  if(on > 0){
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }

  hdr.AddMember("type", type, alloc);

  res.SetObject()
    .AddMember("header", hdr, alloc)
    .AddMember("body", bdy, alloc);

  res.Accept(writer);

  ss.append(buffer.GetString(), buffer.GetLength());
}

}


