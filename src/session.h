#include <string>
#include <memory>

//#include "Session.hpp"

#ifndef _session_for_lws
#define _session_for_lws

namespace raspserver {

enum STATUS {
	INIT=1,
	SESSION=2,
};

struct per_session_data_raspserver {
	STATUS status;
	time_t start_time;
	string *buff;
};

}

#endif /* _session_for_lws */

