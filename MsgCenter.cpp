/******************************************************************************
作者：		何登锋
功能说明:

******************************************************************************/
#include "MsgCenter.h"
#include "MsgDefine.h"
#include "MsgClient.h"

#include "zmq/zmq.h"

const char* MsgCenter::MESSAGE_CENTER_ENDPOINT = "inproc://message_center";


/**************************************************************************
作者: 何登锋
功能描述:
	注册消息客户系统。
参数说明:
返回值:
**************************************************************************/
bool MsgCenter::register_client(thread::id id, MsgClient &client)
{
	pair<MsgCenterMap::iterator, bool> ret;

	lock_guard<std::recursive_mutex> lock(_mutex);

	ret = _client_pool.insert(MsgCenterPair(id, &client));

	return ret.second;
}



/**************************************************************************
作者: 何登锋
功能描述:
	返回消息客户系统。
参数说明:
返回值:
**************************************************************************/
MsgClient *MsgCenter::find_client(thread::id id)
{

	MsgCenterMap::iterator it;

	lock_guard<std::recursive_mutex> lock(_mutex);

	it = _client_pool.find(id);

	if(it == _client_pool.end())
	{
		return NULL;
	}

	return (*it).second;
}


/**************************************************************************
作者: 何登锋
功能描述:
	清除所有消息客户系统的消息队列。
参数说明:
返回值:
**************************************************************************/
void MsgCenter::clear_all_message_queue(void)
{
	//todo:此处需要注意死锁
	lock_guard<std::recursive_mutex> lock(_mutex);

	if(_client_pool.empty() == true)
	{

		return;
	}

	MsgCenterMap::iterator it = _client_pool.begin();
	for(; it!=_client_pool.end(); ++it)
	{
		((*it).second)->clear_message_queue();
	}

}



/**************************************************************************
作者: 何登锋
功能描述:
	该方法将把消息派发给其他消息客户系统。
参数说明:
返回值:
**************************************************************************/
bool MsgCenter::dispatch_message(const MsgClient &client, const MsgEntity &msg)
{
	//todo:此处需要注意死锁
	lock_guard<std::recursive_mutex> lock(_mutex);

	MsgCenterMap::iterator it = _client_pool.begin();
	for(; it!=_client_pool.end(); ++it)
	{
		if(*((*it).second) == client)
		{
			continue;
		}

		// 这里直接将消息添加到客户端的接受队列当中，并不管该消息客户端是否订阅了该
		// 消息。如果需要扩展的话也不是太难，但是也有一个比较的过程，该过程并不能提
		// 高很多效率，因此暂时没有实现。
		((*it).second)->add_message_to_receive_queue(msg);
	}
	return true;
}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
MsgCenter::MsgCenter()
{

}



/******************************************************************************
作者：何登锋
功能描述：
参数说明：
返回值：
******************************************************************************/
MsgCenter::~MsgCenter()
{
	// 析够所有的消息客户系统。
	MsgCenterMap::iterator it = _client_pool.begin();
	for(; it != _client_pool.end(); ++it)
	{
		delete (*it).second;
	}
}

