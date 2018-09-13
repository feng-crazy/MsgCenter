/*
 * testMessage.h
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */

#ifndef UNITTEST_MESSAGETYPE_H_
#define UNITTEST_MESSAGETYPE_H_


enum MessageType
{
	MSG_NONE,
	MSG_SYSTEM_STARTUP,  // 系统启动消息
	MSG_SYSTEM_TIME_1,   // 系统每秒一次的定时消息
	MSG_TEST_TARGET_1,
	MSG_TEST_TARGET_2,
	MSG_TEST_THREAD_1,
	MSG_TEST_THREAD_2,
};


#endif /* UNITTEST_MESSAGETYPE_H_ */
