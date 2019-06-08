
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

using namespace std;
using namespace rapidjson;

#ifndef HEADERFILE_ACTION
#define HEADERFILE_ACTION

namespace raspserver {

class Action {
	public:
		virtual ~Action(){
			//BOOST_LOG_TRIVIAL(info) << "~Action...";
		};
		virtual void doAction(unique_ptr<Document> rq, string& ss){}
};

}

#endif
