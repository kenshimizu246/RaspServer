
#include <string>
#include <exception>
#include <stdexcept>

#include <libwebsockets.h>
#include <rapidjson/document.h>

#include "actions/Action.hpp"

using namespace rapidjson;

namespace raspserver {

class ActionFactory {
	private:

	public:
		ActionFactory();
		~ActionFactory();
		Action* getActionHandler(string& actionName);
};

}
