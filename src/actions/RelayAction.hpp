
#include <string>
#include <exception>
#include <stdexcept>

#include <libwebsockets.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "Action.hpp"

using namespace std;
using namespace rapidjson;

namespace raspserver {

class RelayAction 
		: public Action {
	private:

	public:
		RelayAction();
		~RelayAction();
		void doAction(unique_ptr<Document> rq, string& ss);
};

}

