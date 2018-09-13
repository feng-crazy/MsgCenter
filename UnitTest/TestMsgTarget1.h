/*
 * TestMsgTarget1.h
 *
 *  Created on: Sep 13, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_TESTMSGTARGET1_H_
#define UNITTEST_TESTMSGTARGET1_H_

#include "../MsgCommon.h"
#include "MessageType.h"


class TestMsgTarget1 : public CmdTarget
{
public:
	int message_handle(const MsgEntity &msg)
	{
		printf("TestMsgTarget1 message_handle msg id:%d ,%s\n",msg.id,msg.wparam);
		return 0;
	};

	void object_test()
	{
		printf("MobjectTest1 object_test publish_thread_message\n");
		MsgEntity msg;
		msg.id = MSG_TEST_TARGET_1;
		string str = "TestMsgTarget1";
		msg.wparam = (WParam)(str.c_str());
		msg.wsize = str.size() + 1;
		msg.priority = MP_NORMAL;
		msg.lparam = 0;
		msg.lsize = 0;
		publish_thread_message(msg);
	};

	TestMsgTarget1()
	{
		subscribe(MSG_TEST_TARGET_2, *this);
		subscribe(MSG_SYSTEM_TIME_1, *this);
	};

	~TestMsgTarget1()
	{
		unsubscribe(MSG_TEST_TARGET_2, *this);
		unsubscribe(MSG_SYSTEM_TIME_1, *this);
	};

	virtual void SetUp(){ };

	virtual void TearDown() { };
};



#endif /* UNITTEST_TESTMSGTARGET1_H_ */
