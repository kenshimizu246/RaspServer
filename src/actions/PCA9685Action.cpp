
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
#include "../AppCtx.hpp"
#include "PCA9685Action.hpp"

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
  unsigned int id = 0;

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  if(!(*rq)["body"].HasMember("pin")){
    throw invalid_argument("pin is mandatory!");
  }

  if(!(*rq)["body"].HasMember("tick")){
    throw invalid_argument("tick is mandatory!");
  }

  if((*rq)["body"].HasMember("id")){
    id = (*rq)["body"]["id"].GetInt();
  }

  unsigned int pin = (*rq)["body"]["pin"].GetInt();
  unsigned int tick = (*rq)["body"]["tick"].GetInt();

cout << "pin:" << pin << endl;
cout << "tick:" << tick << endl;

  if(raspserver::AppCtx::getInstance().getSizeOfPCA9685() > (id + 1)){
    throw invalid_argument("id is not available in the list!");
  }
  shared_ptr<PCA9685> pca = raspserver::AppCtx::getInstance().getPCA9685(id);

  pca->PWMWrite(pin,0,tick);

  hdr.AddMember("type", type, alloc);

  res.SetObject()
    .AddMember("header", hdr, alloc)
    .AddMember("body", bdy, alloc);

  res.Accept(writer);

  ss.append(buffer.GetString(), buffer.GetLength());
}

}


