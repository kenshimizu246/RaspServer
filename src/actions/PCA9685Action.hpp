
#include <string>
#include <exception>
#include <stdexcept>

#include <libwebsockets.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "Action.hpp"
#include "../AppCtx.hpp"

using namespace std;
using namespace rapidjson;

namespace raspserver {

class PCA9685Action 
		: public Action {
	private:

	public:
		PCA9685Action();
		~PCA9685Action();
		void doAction(unique_ptr<Document> rq, string& ss);
};

}

