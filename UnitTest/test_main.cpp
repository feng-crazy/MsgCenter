/*
 * main.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */


#include <signal.h>

#include "../MFrame.h"

#include "MobjectTest.cpp"
#include "testMthread.cpp"
#include "testSingleton.cpp"
#include "testMsgClient.cpp"
#include "testMsgCenter.cpp"



class testBin : public CmdTarget
{
public:
	testBin()
	{
		MsgCenter *_msg_center = MsgCenterSingleton::instance();

		MsgClient *_client = new MsgClient();
		CmdTarget::set_client(*_client);

		subscribe(MSG_MOBJECT_ONE,*this);
		subscribe(MSG_MOBJECT_TWO,*this);

        MthreadTest1 thread_test1;
		thread_test1.run();

		MthreadTest2 thread_test2;
		thread_test2.run();

		printf("testbin... publish_thread_message\n");

		MsgEntity msg;
		msg.id = MSG_MAIN_MESSAGE;
		string str = "testBin";
		msg.wparam = (WParam)(str.c_str());
		msg.wsize = str.size() + 1;
		msg.priority = MP_NORMAL;
		msg.lparam = 0;
		msg.lsize = 0;
		publish_thread_message(msg);



		while(1)
		{
			this_thread::sleep_for(std::chrono::seconds(1));

			printf("main thread loop .........\n");
			_client->handle_message();
		}
	}
	~testBin()
	{
		unsubscribe(MSG_MOBJECT_ONE,*this);
		unsubscribe(MSG_MOBJECT_TWO,*this);
	}

	int message_handle(const MsgEntity &msg)
	{
		printf("testBin message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
		return 0;
	}
};


//void error_handler(int sig)
//{


//	string s("signal is = ");
//	s += to_string(sig);

//	stack_trace(s.c_str());

//	signal(sig, SIG_DFL);
//	raise(sig);

//}


int main1()
{

//	signal(SIGINT, error_handler);
//	signal(SIGQUIT, error_handler);
//	signal(SIGILL, error_handler);
//	signal(SIGTRAP, error_handler);
//	signal(SIGABRT, error_handler);
//	signal(SIGIOT, error_handler);
//	signal(SIGBUS, error_handler);
//	signal(SIGFPE, error_handler);
//	signal(SIGKILL, error_handler);
//	signal(SIGSEGV, error_handler);
//	signal(SIGTERM, error_handler);
//	signal(SIGSTKFLT, error_handler);
//	signal(SIGPIPE, SIG_IGN);

	testBin *test = new testBin();
	delete test;

	return 0;
}
