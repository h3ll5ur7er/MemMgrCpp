#pragma once
#ifndef MEMORY_MANAGER_INITIALIZED
#define MEMORY_MANAGER_INITIALIZED
#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

#include "DLinkedList.h"
#include <new>
#include <iostream>

using namespace std;

class MemMgr
{
public:
	DLinkedList* mem;
};

static MemMgr* mgr;

static const char* fileMem = "";
static int lineMem = 0;

static long bytesAllocated = 0;
static long maxBytesAllocated = 0;

typedef void*(*myNew)(size_t);
typedef void(*myDelete)(void*);

void * operator new(size_t);
void * operator new[](size_t);
void * operator new(size_t);
void * operator new[](size_t);
void operator delete(void *);
void operator delete[](void *);

void operator delete(void *);
void operator delete[](void *);

extern myNew newFunc;
extern myNew newFuncA;
extern myDelete delFunc;
extern myDelete delFuncA;

void log(void* addr, size_t size);
void loga(void* addr, size_t size);
void init_mgr();
void finalize();

void* log_new(size_t size) throw(std::bad_alloc);

void* log_new_a(size_t size) throw(std::bad_alloc);
void* init_new(size_t size) throw(std::bad_alloc);
void* init_new_a(size_t size) throw(std::bad_alloc);
void log_del(void* p);
void log_del_a(void* p);

extern myNew newFunc = init_new;
extern myNew newFuncA = init_new_a;
extern myDelete delFunc = log_del;
extern myDelete delFuncA = log_del_a;

inline void* operator new(size_t size) throw(std::bad_alloc)
{
	return newFunc(size);
}

inline void* operator new[](size_t size) throw(std::bad_alloc)
{
	return newFuncA(size);
}

inline void operator delete(void *p) throw()
{
	delFunc(p);
}

inline void operator delete[](void *p) throw()
{
	delFuncA(p);
}
bool hack(const char* file, int line);



#define new hack(__FILE__,__LINE__)&&0?NULL:new
#define delete hack(__FILE__,__LINE__)&&0?NULL:delete
#endif

#endif