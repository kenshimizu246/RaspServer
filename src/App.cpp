
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <libwebsockets.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "session.h"
#include "Config.hpp"
#include "ActionFactory.hpp"
#include "actions/Action.hpp"
#include "Session.hpp"
#include "SessionManager.hpp"
#include "AppWebSocketWriter.hpp"
#include "AppCtx.hpp"

#include "PCA9685.hpp"
#include "HcSr04.hpp"

#include <wiringPi.h>

#define HERTZ 50

#define DAEMON_NAME "simpledaemon"

using namespace std;
using namespace rapidjson;

namespace raspserver {

class RaspServer {
	public:
		RaspServer();
		void init();
		void daemonize(string rundir, string pidfile);
		void run();

	private:
		static void signal_handler(int sig);
		static void sighandler(int sig);
		static void daemonShutdown();
		static int  callback_raspserver(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

		struct lws_protocols *pp;
		struct lws_context_creation_info info;
		int force_exitx;
		static int force_exit;
		static int pidFilehandle;
		static struct lws_context *context;
		SessionManager sessionManager;
};

int RaspServer::force_exit = 0;
int RaspServer::pidFilehandle;
struct lws_context * RaspServer::context;

RaspServer::RaspServer(){
	pp = new lws_protocols[2];
	pp[0].name = "quote";
	pp[0].callback = callback_raspserver;
	pp[0].per_session_data_size = sizeof(struct per_session_data_raspserver);
	pp[0].rx_buffer_size = 8000;
	pp[1].name = NULL;
	pp[1].callback = NULL;
	pp[1].per_session_data_size = 0;
	pp[1].rx_buffer_size = 0;
	force_exitx = 0;
}

void RaspServer::daemonShutdown(){
//	(pidFilehandle);
}

void RaspServer::signal_handler(int sig){
	switch(sig){
		case SIGHUP:
		case SIGINT:
		case SIGTERM:
			daemonShutdown();
			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}
}

void RaspServer::daemonize(string rundir, string pidfile) {
	int pid, sid, i;
	char str[10];
	struct sigaction newSigAction;
	sigset_t newSigSet;

	if (getppid() == 1) {
		return;
	}

	sigemptyset(&newSigSet);
	sigaddset(&newSigSet, SIGCHLD);
	sigaddset(&newSigSet, SIGTSTP);
	sigaddset(&newSigSet, SIGTTOU);
	sigaddset(&newSigSet, SIGTTIN);
	sigprocmask(SIG_BLOCK, &newSigSet, NULL);

	newSigAction.sa_handler = signal_handler;
	sigemptyset(&newSigAction.sa_mask);
	newSigAction.sa_flags = 0;

	sigaction(SIGHUP, &newSigAction, NULL);
	sigaction(SIGTERM, &newSigAction, NULL);
	sigaction(SIGINT, &newSigAction, NULL);

	// fork returns child process id if process is parent or 0 if process is child process
	// this kills the parent process to be independent process and under init/kernel process
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(027);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	for (i = getdtablesize(); i >= 0; --i) {
		close(i);
	}

	i = open("/dev/null", O_RDWR);

	dup(i);

	chdir(rundir.c_str());

	pidFilehandle = open(pidfile.c_str(), O_RDWR|O_CREAT, 0600);

	if (pidFilehandle == -1 ) {
//		BOOST_LOG_TRIVIAL(info) << "daemonize.pidFilehandle" << pidfile;
		exit(EXIT_FAILURE);
	}

	if (lockf(pidFilehandle,F_TLOCK,0) == -1) {
		exit(EXIT_FAILURE);
	}

	sprintf(str,"%d\n",getpid());

	write(pidFilehandle, str, strlen(str));

	close(pidFilehandle);
}

void RaspServer::sighandler(int sig)
{
	force_exit = 1;
	lws_cancel_service(context);
}

void RaspServer::init(){
//	BOOST_LOG_TRIVIAL(info) << "end of server initialization... init";

	int size = 0;
	size = Config::getInstance().getI2CSize();
	for(int i = 0; i < size; i++){
		shared_ptr<I2CConf> p = Config::getInstance().getI2CConf(i);
		shared_ptr<PCA9685> pca = shared_ptr<PCA9685>(new PCA9685());
		pca->Setup(p->address, p->hertz);
		pca->PWMReset();
		raspserver::AppCtx::getInstance().add(pca);
	}

	size = Config::getInstance().getHcSr04ConfSize();
	for(int i = 0; i < size; i++){
		shared_ptr<HcSr04Conf> p = Config::getInstance().getHcSr04Conf(i);
		shared_ptr<HcSr04> hcsr04 = shared_ptr<HcSr04>(new HcSr04(p->pinTrig, p->pinEcho));
		raspserver::AppCtx::getInstance().add(hcsr04);
	}
}

void RaspServer::run(){
	int opts = 0;
	int n = 0;
	unsigned int ms, oldms = 0;   

	/* Deamonize */
	if(Config::getInstance().isDaemon()){
		cout << "daemonize... " << Config::getInstance().getAppDir() << " : " << Config::getInstance().getPidFile() << endl;
		daemonize(Config::getInstance().getAppDir().c_str(), Config::getInstance().getPidFile().c_str());
		cout << "daemonized!" << endl;
	} else {
		cout << "not daemonized!" << endl;
	}

//	init_log();


	Config::getInstance().setSessionManager(&sessionManager);

	memset(&info, 0, sizeof info);
	info.port = Config::getInstance().getPort();
	info.protocols = pp;
	info.ssl_cert_filepath = NULL;
	info.ssl_private_key_filepath = NULL;
	info.extensions = lws_get_internal_extensions();
	info.extensions = NULL;

	info.gid = -1;
	info.uid = -1;
	info.max_http_header_pool = 1;
	info.options = opts;
    
	signal(SIGINT, sighandler);
 
	context = lws_create_context(&info);
	if (context == NULL) {
		lwsl_err("libwebsocket init failed\n");
		return; // -1;
	}
 
	// start conversation monitor...
//	bool stopQt{false};
//	ConversationManager& cm{conversationManager};

/*
	thread qt([&cm,&stopQt]{
		struct timespec rq;

		rq.tv_sec = 1;
		rq.tv_nsec = 0; // 1000000 <- 1ms

		while(!stopQt){
			nanosleep(&rq, NULL);
			cm.checkTimeout();
		}	
	});
*/
 
	n = 0;
	while (n >= 0 && !force_exit) {
		struct timeval tv;

		gettimeofday(&tv, NULL);

		ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
		if ((ms - oldms) > 50) {
			lws_callback_on_writable_all_protocol(context,
				&pp[0]);
			oldms = ms;
		}

		n = lws_service(context, 50);
	}//while n>=0
	//BOOST_LOG_TRIVIAL(info) << "run while out";

	lws_context_destroy(context);
	//BOOST_LOG_TRIVIAL(info) << "lws_context_destroy(context) end...";
}

int RaspServer::callback_raspserver(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len)
{
	struct per_session_data_raspserver *pss = (struct per_session_data_raspserver *)user;
	int n;

	const string type_action("action");
	const string login("login");
	const string heartbeat("heartbeat");

	unique_ptr<Document> rq;

	switch (reason) {
		case LWS_CALLBACK_ESTABLISHED:
			pss->status = INIT;
			pss->buff = new string("");
			time(&(pss->start_time));
			pss->session = Config::getInstance().getSessionManager()->newSession();
			pss->session->writer = shared_ptr<AppWebSocketWriter>(new AppWebSocketWriter(wsi));
			break;

		case LWS_CALLBACK_SERVER_WRITEABLE:
			if (pss->buff->length() < 1) {
				break;
			}

			rq.reset(new Document());

			if ((*rq).Parse<0>(pss->buff->c_str()).HasParseError()){
				// BOOST_LOG_TRIVIAL(info) << "HasParseError...";
			} else if ((*rq).HasMember("header")) {
				if ((*rq)["header"].HasMember("type")){
					if((*rq)["header"]["type"].GetString() == type_action) {
						ActionFactory af;

						if ((*rq)["header"].HasMember("action_name")){
							string actionName((*rq)["header"]["action_name"].GetString());
							unique_ptr<Action> action{af.getActionHandler(actionName)};
							string rstr;
							action->doAction(move(rq), rstr);
						}
					}
				}
			}
			pss->buff->clear();
			break;

		case LWS_CALLBACK_RECEIVE:
			pss->buff->append((const char*)in, len);
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
			break;

		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
			break;

		case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
			if(pss != nullptr){
				delete pss->buff;
				Config::getInstance().getSessionManager()->del(pss->session);
			}

			lwsl_notice("LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: len %d\n", len);
			for (n = 0; n < (int)len; n++) {
				lwsl_notice(" %d: 0x%02X\n", n, ((unsigned char *)in)[n]);
			}
			break;

		default:
			break;
	}
	return 0;
}//callback_raspserver



}


int main(int argc, char **argv)
{
	cout << "Start main..." << endl;

  if ( argc != 2) {
    cerr << "argument count mismatch error." << endl;
    exit(EXIT_FAILURE);
  }
	raspserver::Config::getInstance().load(argv[1]);

	cout << "AppDir:" << raspserver::Config::getInstance().getAppDir() << endl;
	cout << "LogDir:" << raspserver::Config::getInstance().getLogDir() << endl;
	cout << "PidFile:" << raspserver::Config::getInstance().getPidFile() << endl;

	raspserver::RaspServer server;

	server.init();

	cout << "after init..." << endl;

	server.run();
 
//	BOOST_LOG_TRIVIAL(info) << "server.run() end...";

	return 0;
} //main

