/*
 * myFrame.h
 *
 *  Created on: Apr 4, 2018
 *      Author: hdf_123
 */

#ifndef FRAMECOMMON_H_
#define FRAMECOMMON_H_


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <memory>

#include <thread>
#include <mutex>

#include "zmq/zmq.h"

using namespace std;


#ifndef _WIN32

#include <pthread.h>

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <time.h>

void stack_trace(const char *ptr);

#endif


#endif /* FRAMECOMMON_H_ */
