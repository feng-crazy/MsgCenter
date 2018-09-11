/*
 * testMthread.cpp
 *
 *  Created on: Apr 12, 2018
 *      Author: hdf_123
 */
#ifndef _TESTMTHREAD_CPP_
#define _TESTMTHREAD_CPP_

#include "../MFrame.h"


#include "MobjectTest.cpp"
#include "testMessage.h"

class MthreadTest1 : public MThread
{
public:
	MthreadTest1() : MThread(),_test_setup_thread_flag(false),_object_test1(NULL)
	{
		MThread::init();
	};

	~MthreadTest1()
	{

	};

protected:
	 void setup_thread()
	 {
//		 printf("MthreadTest1.setup_thread..%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test1 = new MobjectTest1();
	 };

	 void thread_run()
	 {
		 printf("MthreadTest1 is run ...\n");
		 _object_test1->object_test();
		 this_thread::sleep_for(std::chrono::seconds(1));
	 };

private:
	 bool _test_setup_thread_flag;
	 MobjectTest1 *_object_test1;
};


class MthreadTest2 : public MThread
{
public:
	MthreadTest2() : MThread(),_test_setup_thread_flag(false),_object_test2(NULL)
	{
		MThread::init();
	};

	~MthreadTest2(){};
protected:
	//下面的函数已经在线程内调用
	 void setup_thread()
	 {
//		 printf("MthreadTest2..setup_thread.%p,pthread id:%d\n",this_thread::get_id(),pthread_self());
		 _test_setup_thread_flag = true;
		 _object_test2 = new MobjectTest2();
	 };
	 void thread_run()
	 {
		 printf("MthreadTest2 is run ....\n");
		 _object_test2->object_test();
		 this_thread::sleep_for(std::chrono::seconds(1));
	 };
private:
	 bool _test_setup_thread_flag;
	 MobjectTest2 *_object_test2;
};
#endif
