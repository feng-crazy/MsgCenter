
#ifndef _MOBJECT_TEST_CPP_
#define _MOBJECT_TEST_CPP_

#include "../MFrame.h"
#include "testMessage.h"

class MobjectTest1 : public CmdTarget
{
public:
	int message_handle(const MsgEntity &msg)
	{
		printf("MobjectTest1 message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
		return 0;
	}

	void object_test()
	{
		printf("MobjectTest1 object_test publish_thread_message\n");
		MsgEntity msg;
		msg.id = MSG_MOBJECT_ONE;
		string str = "MobjectTest1";
		msg.wparam = (WParam)(str.c_str());
		msg.wsize = str.size() + 1;
		msg.priority = MP_NORMAL;
		msg.lparam = 0;
		msg.lsize = 0;
		publish_thread_message(msg);
	}

	MobjectTest1()
	{
		subscribe(MSG_MAIN_MESSAGE,*this);
		subscribe(MSG_MOBJECT_TWO,*this);
	};
	~MobjectTest1()
	{
		unsubscribe(MSG_MAIN_MESSAGE,*this);
		unsubscribe(MSG_MOBJECT_TWO,*this);
	};

	virtual void SetUp(){ };
	virtual void TearDown() { };
};


class MobjectTest2 : public CmdTarget
{
public:
	int message_handle(const MsgEntity &msg)
	{
		printf("MobjectTest2 message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
		return 0;
	}

	void object_test()
	{
		printf("MobjectTest2 object_test publish_thread_message\n");

		MsgEntity msg;
		msg.id = MSG_MOBJECT_TWO;
		string str = "MobjectTest2";
		msg.wparam = (WParam)(str.c_str());
		msg.wsize = str.size() + 1;
		msg.priority = MP_NORMAL;
		msg.lparam = 0;
		msg.lsize = 0;
		publish_thread_message(msg);
	}

	MobjectTest2()
	{
		subscribe(MSG_MAIN_MESSAGE,*this);
		subscribe(MSG_MOBJECT_ONE,*this);
	};
	~MobjectTest2()
	{
		unsubscribe(MSG_MAIN_MESSAGE,*this);
		unsubscribe(MSG_MOBJECT_ONE,*this);
	};

	virtual void SetUp(){ };
	virtual void TearDown() { };
};

#endif


