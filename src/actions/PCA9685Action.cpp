
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
#include "../PCA9685.hpp"
#include "PCA9685Action.hpp"

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

using namespace rapidjson;

namespace raspserver {

PCA9685Action::PCA9685Action() {
}

PCA9685Action::~PCA9685Action() {
}

void PCA9685Action::doAction(unique_ptr<Document> rq, string& ss){
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

  if(!(*rq)["body"].HasMember("tick")){
    throw invalid_argument("tick is mandatory!");
  }

  int pin = (*rq)["body"]["pin"].GetInt();
  int tick = (*rq)["body"]["tick"].GetInt();

cout << "pin:" << pin << endl;
cout << "tick:" << tick << endl;

  raspserver::PCA9685::getInstance().PWMWrite(pin,0,tick);

  hdr.AddMember("type", type, alloc);

  res.SetObject()
    .AddMember("header", hdr, alloc)
    .AddMember("body", bdy, alloc);

  res.Accept(writer);

  ss.append(buffer.GetString(), buffer.GetLength());
}

}


