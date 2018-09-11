﻿/******************************************************************************
	  COPYRIGHT: 2008 Edan Instruments, Inc. All rights reserved
	PROJECTNAME: Mx
	    VERSION: 1.0
	   FILENAME: CmdTarget.cpp
	    CREATED: 2008.12.09
	     AUTHOR: ZhangHu
	DESCRIPTION:

******************************************************************************/
#include "CmdTarget.h"
#include "MsgClient.h"
#include "MsgCenter.h"

const int NONE_HANDLE = 0xA5A5A5EF; // 随机数据没有任何意义，不要追究为什么。

/******************************************************************************
作者: 何登锋
功能描述:
参数说明:
返回值:
******************************************************************************/
bool CmdTarget::operator ==(const CmdTarget &object) const
{
	return (this == &object);
}

/******************************************************************************
作者: 何登锋
功能描述:
	消息处理方法，需要处理消息的对象需要重写该方法，注意在处理的消息需要先订阅。
参数说明:
	msg: 消息。
返回值:
	NONE_HANDLE: 未处理该消息。
	其他: 自定义。
******************************************************************************/
int CmdTarget::message_handle(const MsgEntity &msg)
{
	return NONE_HANDLE;
}



/******************************************************************************
作者: 何登锋
功能描述:
	发布一条消息，消息订阅者对象的message_handle方法将被调用。
参数说明:
	msg: 消息。
返回值:
	NONE_HANDLE: 订阅者未处理该消息。
	其他: 自定义。
******************************************************************************/
int CmdTarget::publish_message(const MsgEntity &msg)
{
	if(_client == NULL)
	{
		return -1;
	}

	thread::id id = this_thread::get_id();
	MsgCenter *msg_center = MsgCenterSingleton::instance();
	MsgClient *client = msg_center->find_client(id);
	if(client == NULL)
	{
		printf("Can't find message client!\n");
	}
	if(client != _client)
	{
		// 进来这里就说明，正在运行的线程调用了其他线程创建的对象，并在发线程内消息，
		// 这应该是不允许的(多线成并发访问)，但因为单列的使用，可能会存在该风险。
		// 为了防止该类问题发生，把消息放入接收消息队列，由线程稍候自己执行。
		printf("%s(line %d):, multi-thread calling.\n", __FUNCTION__, __LINE__);
		_client->add_message_to_receive_queue(msg);
		return 0;
	}

	return _client->notify_local_observer(msg);
}



/******************************************************************************
作者: 何登锋
功能描述:
	发布一条消息，消息订阅者对象的message_handle方法将不会被立即调用，只是将该条消息放到接收
	对列中。
参数说明:
	msg: 消息。
返回值:	
******************************************************************************/
void CmdTarget::publish_message_queue(const MsgEntity &msg)
{
	if(_client == NULL)
	{
		return;
	}

	_client->add_message_to_receive_queue(msg);
}



/******************************************************************************
作者: 何登锋
功能描述:
	发布一条消息，本线程中的消息订阅者对象的message_handle方法将会立即调用，将该条消息还将放到
	发送对列中，稍候该消息会被发送到另外线程中去。
参数说明:
	msg: 消息。
返回值:	
******************************************************************************/
void CmdTarget::publish_thread_message(const MsgEntity &msg)
{
	if(_client == NULL)
	{
		return;
	}

	thread::id id = this_thread::get_id();

	MsgCenter *msg_center = MsgCenterSingleton::instance();

	MsgClient *client = msg_center->find_client(id);

	if(client == NULL)
	{
		printf("Can't find message client!\n");
	}

	if(client != _client)
	{
		// 进来这里就说明，正在运行的线程调用了其他线程创建的对象，并在发线程内消息，
		// 这应该是不允许的(多线成并发访问)，但因为单列的使用，可能会存在该风险。
		// 为了防止该类问题发生，把消息放入接收消息队列，由线程稍候自己执行。
		printf("%s(line %d):, multi-thread calling.\n", __FUNCTION__, __LINE__);
		_client->add_message_to_receive_queue(msg);
	}
	else
	{
		_client->notify_local_observer(msg);
	}

	_client->add_message_to_send_queue(msg);

}



/******************************************************************************
作者: 何登锋
功能描述:
	发布一条消息，只是将该条消息还将放到发送对列中，稍候该消息会被发送到另外线程中去。
参数说明:
	msg: 消息。
返回值:	
******************************************************************************/
void CmdTarget::publish_thread_message_only(const MsgEntity &msg)
{
	if(_client == NULL)
	{
		return;
	}

	_client->add_message_to_send_queue(msg);
}



/******************************************************************************
作者: 何登锋
功能描述:
	将跨线程消息立即发送出去。
参数说明:
返回值:	
******************************************************************************/
void CmdTarget::flush_thread_message(void)
{
	if(_client == NULL)
	{
		return;
	}

	_client->handle_send_queue();
}



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一条消息。
参数说明:
	id: 消息的标示号。
	object: 关注消息的对象引用。
返回值:
	订阅成功返回true，否则false，一个对象重复订阅一条消息的时候会返回false。
******************************************************************************/
bool CmdTarget::subscribe(Message id, CmdTarget &object)
{
	if(_client == NULL)
	{
		return false;
	}

	return _client->register_observer(id, object);
}



/******************************************************************************
作者: 何登锋
功能描述:
	订阅一组消息。
参数说明:
	start_id: 消息组中第一个消息的ID。
	end_id: 消息组中最后一个消息的ID。
	object: 关注消息的对象引用。
返回值:	
	订阅成功返回true，否则false，一个对象重复订阅消息的时候会返回false。
******************************************************************************/
void CmdTarget::subscribe(Message start_id, Message end_id, CmdTarget &object)
{
	for(unsigned int id=start_id; id<=end_id; id++)
	{
		subscribe(id, object);
	}
}



/******************************************************************************
作者: 何登锋
功能描述:
	退订一条消息。
参数说明:
	id: 消息的ID。
	object: 关注消息的对象引用。
返回值:	
	订阅成功返回true，否则false。
******************************************************************************/
bool CmdTarget::unsubscribe(Message id, CmdTarget &object)
{
	if(_client == NULL)
	{
		return false;
	}

	return _client->unregister_observer(id, object);
}



/******************************************************************************
作者: 何登锋
功能描述:
	退订一组消息。
参数说明:
	start_id: 消息组中第一个消息的ID。
	end_id: 消息组中最后一个消息的ID。
	object: 关注消息的对象引用。
返回值:	
	订阅成功返回true，否则false。
******************************************************************************/
void CmdTarget::unsubscribe(Message start_id, Message end_id, CmdTarget &object)
{
	for(unsigned int id=start_id; id<=end_id; id++)
	{
		unsubscribe(id, object);
	}
}



/******************************************************************************
作者: 何登锋
功能描述:
	设置该CmdTarget所属的消息客户对象。该对象不是接口，应用层不应直接调用，要是导致CmdTarget
	关联到另外线程的客户对像，会造成非常麻烦的事情。请注意不要直接调用它！
参数说明:
	client: 消息客户对象引用。
返回值:	
******************************************************************************/
void CmdTarget::set_client(MsgClient &client)
{
	_client = &client;
}



/******************************************************************************
作者：何登锋
功能描述：
	清空发送队列和接受队列。
参数说明：
返回值：
******************************************************************************/
void CmdTarget::clear_message_queue(void)
{
	if(_client == NULL)
	{
		return;
	}

	return _client->clear_message_queue();
}




/******************************************************************************
作者: 何登锋
功能描述:
	构造方法。
参数说明:
	parent: 父对象。
返回值:	
******************************************************************************/
CmdTarget::CmdTarget()
{
	// 得到当前线程的ID号，这个ID也是当前线程所对应消息客户对象的ID。
	thread::id id = this_thread::get_id();
	
	// 从消息中心中检索出消息客户对象。
	MsgCenter *msg_center = MsgCenterSingleton::instance();
	_client = msg_center->find_client(id);
	
	if(_client == NULL)
	{
		printf("Can't find message client!\n");
	}
}



/******************************************************************************
作者: 何登锋
功能描述:
	析构方法。
参数说明:
返回值:	
******************************************************************************/
CmdTarget::~CmdTarget()
{

}

