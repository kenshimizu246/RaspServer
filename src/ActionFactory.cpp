
#include <string>
#include <exception>
#include <stdexcept>

#include <rapidjson/document.h>

#include "ActionFactory.hpp"
#include "actions/Action.hpp"
#include "actions/NullAction.hpp"
#include "actions/ErrorAction.hpp"
#include "actions/PCA9685Action.hpp"
#include "actions/RelayAction.hpp"

using namespace std;
using namespace rapidjson;

namespace raspserver {

ActionFactory::ActionFactory(){
}

ActionFactory::~ActionFactory(){
}

Action* ActionFactory::getActionHandler(string& actionName){
	const string move("move");
	const string onoff("onoff");

	if (actionName == move) {
		return new PCA9685Action();
	} else if (actionName == onoff) {
		return new RelayAction();
	}
	return new NullAction();
}

}

