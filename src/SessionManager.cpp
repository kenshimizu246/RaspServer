
#include <string>
#include <iostream>
#include <mutex>
#include <map>
#include <exception>
#include <stdexcept>
#include <time.h>
#include <uuid/uuid.h>

#include "Session.hpp"
#include "SessionManager.hpp"

namespace raspserver {

SessionManager::SessionManager() {
}

SessionManager::~SessionManager() {
	//clean maps
}

int SessionManager::count(){
	return sessions.size();
}

shared_ptr<Session> SessionManager::newSession(){
	shared_ptr<Session> s;

	lock_guard<mutex> lock(mtx);

	uuid_t u;
  char buff[37];
  uuid_generate(u);
  uuid_unparse(u, buff);
  string id(buff);

	s = shared_ptr<Session>(new Session(id, true));
	sessions[id] = s;

	return s;
}

void SessionManager::add(shared_ptr<Session> s){
	lock_guard<mutex> lock(mtx);
	sessions[s->id] = s;
}

void SessionManager::del(shared_ptr<Session> s){
}

bool SessionManager::isValid(string sessionId){
	auto found = sessions.find(sessionId);
	return found != end(sessions);
}

void SessionManager::invalidate(time_t& t){
	for(auto itr = sessions.begin(); itr != sessions.end(); ++itr) {
		shared_ptr<Session> ss = itr->second;

		if(ss->last_accessed < t){
			ss->valid = false;
		}
	}
}

void SessionManager::clean(){
	vector<shared_ptr<Session>> invalid;
	for(auto itr = sessions.begin(); itr != sessions.end(); ++itr) {
		shared_ptr<Session> ss = itr->second;

		if(!ss->valid){
			invalid.push_back(ss);
		}
	}
//	for(shared_ptr<Session> ss : invalid) {
//		logout(ss->userId);
//	}
}

shared_ptr<Session> SessionManager::getSessionBySessionID(string sessionid){
	shared_ptr<Session> s;

	lock_guard<mutex> lock(mtx);
	auto iter = sessions.find(sessionid);
	if(iter != end(sessions)){
		s = iter->second;
	}
	return s;
}

void SessionManager::setMaxSession(int cnt){
	maxSession = cnt;
}

int SessionManager::getMaxSession(){
	return maxSession;
}


//void SessionManager::update(ConversationChangeEvent event){
//	string c_client("client");
//	string c_trader("trader");
//
//	BOOST_LOG_TRIVIAL(info) << "SessionManager.update" ;
//	shared_ptr<Conversation> c = event.getConversation();
//
//	for(auto itr = sessions.begin(); itr != sessions.end(); ++itr) {
//		shared_ptr<Session> ss = itr->second;
//		BOOST_LOG_TRIVIAL(info) << "SessionManager.update::ss.role:" << ss->role
//														<< ":ss.userId:" << ss->userId
//														<< ":c.userId:" << c->userId
//														<< ":sessionid:" << ss->id;
//		if(!ss->valid){
//			BOOST_LOG_TRIVIAL(info) << "SessionManager.update::invalid:";
//			continue;
//		}
//		if(ss->role == c_trader
//				|| (ss->role == c_client && ss->userId == c->userId)){
//			if(ss->writer){
//				BOOST_LOG_TRIVIAL(info) << "SessionManager.update::write is ok";
//				string st;
//				c->toJsonString(st, ss);
//				ss->writer->write(st);
//			}else{
//				BOOST_LOG_TRIVIAL(info) << "SessionManager.update::write is not ok";
//			}
//		}
//	}
//	BOOST_LOG_TRIVIAL(info) << "SessionManager.update done";
//}

}
