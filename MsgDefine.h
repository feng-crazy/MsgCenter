
/******************************************************************************
作者：	何登锋
功能说明：
	消息相关数据原型定义。
	注意事项：由于消息本身使用了枚举来定义，因此它是个整数，每个消息ID到定义被分在不同的文件中，
	这样就有可能冲突，因此在每个消息定义文件的起始处都说明了它们的范围，使用的时候请注意。
******************************************************************************/
#ifndef _MSG_DEFINE_H
#define _MSG_DEFINE_H

#include <algorithm>
#include "FrameCommon.h"
using namespace std;


typedef long WParam;
typedef long LParam;
typedef unsigned Message;

// 消息的优先级，用于确定将消息放到接收线程队列的队首还是队尾，NORMAL放在队尾
// PRIORITY放在队首，队首消息会先得到处理。
enum MsgPriority
{
	MP_NORMAL,
	MP_PRIORITY
};

/******************************************************************************
MsgEntity的使用问题：
publish_thread_message为线程间发送消息，publish_message为线程内发送消息
（1）在publish_thread_message()函数中使用MsgEntity，
	wparam表示地址，wsize指出wparam所指向区域的大小。
	lparam表示地址，lsize指出lparam所指向区域的大小。
（2）在publish_message()函数中使用MsgEntity，
	wparam、wsize、	lparam、lsize四个参数没有太大意义。
	可以按照实际情况来规定各个参数的含义。
******************************************************************************/
struct MsgEntity
{
	MsgEntity() : id(0), priority(MP_NORMAL), wparam(0), lparam(0),
			wsize(0), lsize(0)
	{
	}

	void reset(void)
	{
		id = 0;
		priority = MP_NORMAL;
		wparam = 0;
		lparam = 0;
		wsize = 0;
		lsize = 0;
	}
	unsigned id; // 消息的ID号。
	MsgPriority priority;    // 消息优先级。
	WParam wparam;  // 第一个参数， 该参数只存放了整数，该整数代表的意义，根据具体情况而定。
	LParam lparam;  // 第二个参数，意义同wparm。
	unsigned wsize; // 当wparam的值为一个地址，且该消息需要发送给其他线程，该值指出wparam所指向区域的大小。
	unsigned lsize; // 意义类同wsize。
};


// 消息系统内部使用，用于暂存MsgEntity的数据。
// 比如：当一条消息发送给其他线程的时候，它会马上返回，而该条消息的内容就会缓存到该结构中。
struct MessageEntity
{
	MsgEntity msg;

	static const unsigned BUFF_SIZE = 512; // 限定固定大小为512字节，当然也可以扩充，
	                                       // 不过跨线程的消息不要太大。当消息的长度超过512字节的时候，
	                                       // 后面的部分将被截掉
	char wbuff[BUFF_SIZE];
	char lbuff[BUFF_SIZE];

	MessageEntity()
	{

	}

	MessageEntity &operator=(const MsgEntity &msg_entity)
	{
		msg = msg_entity;

		for(unsigned i=0; i<BUFF_SIZE; i++)
		{
			wbuff[i] = 0;
			lbuff[i] = 0;
		}

		// 拷贝msg.wparam指向的msg.wsize个数据。
		if(msg.wsize > 0)
		{
#ifndef NDEBUG
			if(msg.wsize > BUFF_SIZE)
			{
				printf("Data length of asynchronic message is too long!\n");
//				stack_trace();
			}
#endif
			copy((char*)msg_entity.wparam, (char*)msg_entity.wparam +
					((msg.wsize > BUFF_SIZE) ? BUFF_SIZE : msg.wsize), wbuff);
			msg.wparam = (WParam)wbuff;
			if(msg.wsize > BUFF_SIZE)
			{
				msg.wsize = BUFF_SIZE;
			}
		}

		// 拷贝msg.lparam指向的msg.lsize个数据。
		if(msg.lsize > 0)
		{
#ifndef NDEBUG
			if(msg.lsize > BUFF_SIZE)
			{
				printf("Data length of asynchronic message is too long!");
			}
#endif
			copy((char*)msg_entity.lparam, (char*)msg_entity.lparam +
					((msg.lsize > BUFF_SIZE) ? BUFF_SIZE : msg.lsize), lbuff);
			msg.lparam = (LParam)lbuff;
			if(msg.lsize > BUFF_SIZE)
			{
				msg.lsize = BUFF_SIZE;
			}
		}

		return *this;
	}
};


#endif

