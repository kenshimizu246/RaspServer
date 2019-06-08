
#include <string>
#include <exception>
#include <stdexcept>

#include <rapidjson/document.h>

#include "ActionFactory.hpp"
#include "actions/Action.hpp"
#include "actions/NullAction.hpp"
#include "actions/ErrorAction.hpp"

using namespace std;
using namespace rapidjson;

namespace raspserver {

ActionFactory::ActionFactory(){
}

ActionFactory::~ActionFactory(){
}

Action* ActionFactory::getActionHandler(string& actionName){
	const string move("move");

	if (actionName == move) {
		return new NullAction();
	}
	return new NullAction();
}

}

