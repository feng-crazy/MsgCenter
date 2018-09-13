

/******************************************************************************
作者：		何登锋
功能说明：
	消息缓冲池，由于消息队列中的消息需要不停地申请与释放，如果直接使用new和delete操作
	会影响消息的处理速度，因此构建这个缓冲池来管理消息体。该缓冲池会被放到MsgClient内部。
	这些缓冲池保存的消息体只会不断创建，而不会被删除，这些消息体供线程间发送消息时使用，由于线程
	间的消息发送次数会有限度，因此这个缓冲池的大小不会无限增长。
******************************************************************************/
#ifndef MSG_POOL_H
#define MSG_POOL_H

#include <algorithm>
#include <list>

#include "StandardHead.h"
#include "MsgDefine.h"



class MsgPool
{
	public:
		// 从缓冲池中得到一个消息体。
		MessageEntity &alloc_message(void);

		// 归还消息体。
		void free_message(MessageEntity &entity);

		MsgPool(int size = 50);
		~MsgPool();

	private:
		std::list<MessageEntity*> _pool;
};



/******************************************************************************
作者: 何登锋
功能描述:
	向缓冲池申请一条消息体，如果_pool中没有消息体则重新构造一条返回。
参数说明:
返回值:
******************************************************************************/
inline MessageEntity &MsgPool::alloc_message(void)
{
	MessageEntity * entity = NULL;

	if(_pool.empty() == true)
	{
		entity = new MessageEntity();
		return *entity;
	}

	entity = _pool.front();
	_pool.pop_front();

#ifndef NDEBUG
	if(_pool.size() > 1000)
	{
		printf("%d\n", _pool.size());
	}
#endif

	return *entity;
}



/******************************************************************************
作者: 何登锋
功能描述:
参数说明:
返回值:
******************************************************************************/
inline void MsgPool::free_message(MessageEntity &entity)
{
	_pool.push_back(&entity);

#ifndef NDEBUG
	if(_pool.size() > 1000)
	{
		printf("%d\n", _pool.size());
	}
#endif
}



/******************************************************************************
作者: 何登锋
功能描述:
参数说明:
返回值:
******************************************************************************/
inline MsgPool::MsgPool(int size)
{
	for(int i=0; i<size; i++)
	{
		_pool.push_back(new MessageEntity());
	}
}



/******************************************************************************
作者: 何登锋
功能描述:
参数说明:
返回值:
******************************************************************************/
inline MsgPool::~MsgPool()
{
	MessageEntity *entity = NULL;

	while(_pool.empty() == false)
	{
		entity = _pool.front();
		delete entity;
		_pool.pop_front();
	}
}



#endif // MSG_POOL_H

