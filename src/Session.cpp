#include <sstream>
#include <vector>
#include <time.h>

#include "Session.hpp"

using namespace std;

namespace raspserver {

Session::Session(string id, bool valid)
	:id(id),valid(valid){
	time(&start_time);
	time(&last_accessed);
}

Session::Session(string id, bool valid, time_t start_time)
	:id(id),valid(valid),start_time(start_time){
	time(&last_accessed);
}

Session::~Session() {
}

void Session::update(){
	time(&last_accessed);
}

}

