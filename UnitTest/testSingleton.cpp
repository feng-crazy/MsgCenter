#ifndef _TESTSINGLETON_CPP_
#define _TESTSINGLETON_CPP_

#include "../MFrame.h"



class _testSingleton
{
	public:
		void run(){cout << "testSingleton::run" << endl;};
		~_testSingleton(){cout << "destructor" << endl;};

	private:
		_testSingleton(){cout << "constructor" << endl;};
		DECLARE_SINGLETON_CLASS(_testSingleton);
};

typedef Singleton<_testSingleton> testSingleton;


//int main()
//{
//	testSingleton::instance()->run();
//	testSingleton::instance()->run();
//	testSingleton::instance()->run();
//	return 0;
//}

#endif
