
#ifndef _sessionmanager_h
#define _sessionmanager_h

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <exception>
#include <stdexcept>
#include <memory>

#include "Session.hpp"

using namespace std;

namespace raspserver {

class SessionManager { // : public ConversationChangeListener {
	private:
		map<string, shared_ptr<Session>> sessions;
		mutex mtx;
		int maxSession = 100;

	public:
		SessionManager();
		~SessionManager();
		int count();
    shared_ptr<Session> newSession();
		void add(shared_ptr<Session> s);
		void del(shared_ptr<Session> s);
		bool isValid(string sessionId);
		void invalidate(time_t& t);
		void clean();
		void setMaxSession(int cnt);
		int getMaxSession();
		shared_ptr<Session> getSessionBySessionID(string sessionid);
//	void update(ConversationChangeEvent event);

		// timeout
};

}

#endif /* _sessionmanager_h */

