/*
 * FrameCommon.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: hdf_123
 */

#include "FrameCommon.h"

#ifndef _WIN32
/******************************************************************************
AUTHOR: 何登峰
DESCRIPTION:
	打印当前线程的函数栈，注意使用该功能时，编译参数中不能打开-O选项，且加上-rdynamic -ldl
PARAMETERS:
	无
RETURN:
	无
TODO:
	已知使用arm-linux-gcc-3.4.1+glibc-2.3.2编译出的程序运行时打印不出虚函数
******************************************************************************/
void stack_trace(const char *ptr)
{
	const int max_depth = 32;
	void *trace[max_depth];
	int stack_depth = backtrace(trace, max_depth);
	char log_str[100];   // 每条错误信息不能超过这个长度

	flockfile(stdout);

	////////////////////////////////////////////////////////
	// 开始搜集错误信息
	////////////////////////////////////////////////////////
	time_t t = time(NULL);
	strftime(log_str, sizeof(log_str), "\n\n%Y-%m-%d %H:%M:%S\n", localtime(&t));
	printf("%s", log_str);

	for(int i=0; i<stack_depth; i++)
	{
		Dl_info dlinfo;
		if(!dladdr(trace[i], &dlinfo))
		{
			snprintf(log_str, sizeof(log_str), "@ %p\n", trace[i]);
			printf("%s", log_str);
			continue;
		}

		const char *symname = dlinfo.dli_sname;

		int status;
		char *demangled = __cxxabiv1::__cxa_demangle(symname, NULL, NULL, &status);
		if((status == 0) && demangled)
		{
			symname = demangled;
		}

		if(symname)
		{
			snprintf(log_str, sizeof(log_str), "@ %s\n", symname);
			printf("%s", log_str);
		}
		else
		{
			snprintf(log_str, sizeof(log_str), "@ %p\n", trace[i]);
			printf("%s", log_str);
		}

		if(demangled)
		{
			free(demangled);
		}
	}

	printf("%s", ptr);

	funlockfile(stdout);
}
#endif
