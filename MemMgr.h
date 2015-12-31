//#pragma once
#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
#include <new>
#include <iostream>

using namespace std;


enum AllocationType
{
	Variable,
	Array
};

struct Node
{
	int index;
	void* address;
	AllocationType type;
	size_t t_size;
	const char* file;
	int line;

	Node* next;
	Node* prev;

	Node(){ address = nullptr; t_size = -1; next = nullptr; prev = nullptr; }
	Node(void* memoryAddress, size_t size) { address = memoryAddress; t_size = size; next = nullptr; prev = nullptr; }
};

struct DLinkedList
{
	Node* first;
	Node* last;
	DLinkedList()
	{
		first = nullptr; last = nullptr;
	}

	static Node* create(void* address, size_t size, AllocationType aType, const char* fileName, int lineNumber)
	{
		static int counter = 0;
		Node* node = static_cast<Node*>(malloc(sizeof(Node)));
		node->index = counter++;
		node->address = address;
		node->t_size = size;
		node->type = aType;
		node->file = fileName;
		node->line = lineNumber;
		node->next = nullptr;
		node->prev = nullptr;
		return node;
	}

	int addFirst(Node* node)
	{
		if (first == nullptr)
		{
			first = node;
			last = node;
			return 0;
		}
		first->prev = node;
		node->next = first;
		first = node;
		return node->index;
	}
	int addLast(Node* node)
	{
		if (last == nullptr)
		{
			first = node;
			last = node;
			return 0;
		}
		last->next = node;
		node->prev = last;
		last = node;
		return node->index;
	}
	int addAt(Node* node, int i)
	{
		Node* current = first;
		int index = 0;
		while (index <i)
		{
			if (current->next == nullptr) return -1;
			current = current->next;
			index++;
		}
		node->prev = current->prev;
		current->prev->next = node;
		current->prev = node;
		node->next = current;
		return i;
	}
	int removeFirst()
	{
		if (first == last)
		{
			first = nullptr;
			last = nullptr;
			return 0;
		}
		Node* memory = first;
		first = first->next;
		first->prev = nullptr;
		free(memory);
		return 1;
	}
	int removeLast()
	{
		if (first == last)
		{
			first = nullptr;
			last = nullptr;
			return 0;
		}
		Node* memory = last;
		last = last->prev;
		last->next = nullptr;
		free(memory);
		return 1;
	}
	int removeAt(int i)
	{
		if (first == last && i == 0)
		{
			first = nullptr;
			last = nullptr;
			return 0;
		}
		Node* current = first;
		int index = 0;
		while (index <i && current->next)
		{
			if (current->next == nullptr) return -1;
			current = current->next;
			index++;
		}

		if (current->prev != nullptr)
		{
			current->prev->next = current->next;
		}
		else
		{
			first = current->next;
		}
		if (current->next != nullptr)
		{
			current->next->prev = current->prev;
		}
		else
		{
			last = current->prev;
		}
		free(current);
		return i;
	}
	int size()
	{
		int i = 0;
		Node* a = first;
		while (a != nullptr)
		{
			i++;
			a = a->next;
		}
		return i;
	}
	Node* get(int i)
	{
		Node* current = first;
		int index = 0;
		while (index <i && current->next)
		{
			if (current->next == nullptr) return nullptr;
			current = current->next;
			index++;
		}

		return current;
	}

	int find(void* address)
	{
		Node* current = first;
		int index = 0;
		while (current != nullptr)
		{
			if (current->address == address) return index;
			if (current->next == nullptr) return -1;
			current = current->next;
			index++;
		}
		cout << "invalid search";
		return -1;
	}

	void print()
	{
		if (first == nullptr)
		{
			cout << "{}";
			return;
		}

		Node* n = first;
		cout << "{\r\n  ";
		while (n != nullptr)
		{
			cout << "[#" << n->index << "\tin :" << n->file << ":" << n->line << " @ " << n->address << " size: " << n->t_size << " bytes ]";

			if (n->next != nullptr) cout << ",\r\n  ";
			else
			{
				cout << "\r\n}";
			}
			n = n->next;
		}
	}
};

class MemMgr
{
public:
	DLinkedList* mem;
};

void * operator new(size_t);
void * operator new[](size_t);
void * operator new(size_t);
void * operator new[](size_t);
void operator delete(void *);
void operator delete[](void *);

void operator delete(void *);
void operator delete[](void *);

typedef void*(*myNew)(size_t);
typedef void(*myDelete)(void*);

extern myNew newFunc;
extern myNew newFuncA;
extern myDelete delFunc;
extern myDelete delFuncA;

static MemMgr* mgr;

static const char* fileMem = "";
static int lineMem = 0;

static long bytesAllocated = 0;
static long maxBytesAllocated = 0;

void log(void* addr, size_t size);
void loga(void* addr, size_t size);
void init_mgr();
void finalize();

void* log_new(size_t size) throw(std::bad_alloc)
{
	//cout<<"test new"<<endl;
	void* p = malloc(size);
	if (!p) throw bad_alloc();

	log(p, size);

	bytesAllocated += size;
	if (bytesAllocated>maxBytesAllocated)
		maxBytesAllocated = bytesAllocated;

	return p;
}
void* log_new_a(size_t size) throw(std::bad_alloc)
{
	//cout<<"test new[]"<<endl;
	void* p = malloc(size);
	if (!p) throw bad_alloc();

	loga(p, size);

	bytesAllocated += size;
	if (bytesAllocated>maxBytesAllocated)
		maxBytesAllocated = bytesAllocated;

	return p;
}
void* init_new(size_t size) throw(std::bad_alloc)
{
	//cout<<"test first new"<<endl;
	void* p = malloc(size);
	if (!p) throw bad_alloc();

	init_mgr();
	log(p, size);

	newFunc = log_new;
	newFuncA = log_new_a;

	bytesAllocated += size;
	if (bytesAllocated>maxBytesAllocated)
		maxBytesAllocated = bytesAllocated;

	return p;
}
void* init_new_a(size_t size) throw(std::bad_alloc)
{
	//cout<<"test first new[]"<<endl;
	void* p = malloc(size);
	if (!p) throw bad_alloc();

	init_mgr();
	loga(p, size);

	newFunc = log_new;
	newFuncA = log_new_a;

	bytesAllocated += size;
	if (bytesAllocated>maxBytesAllocated)
		maxBytesAllocated = bytesAllocated;

	return p;
}
void log_del(void* p)
{
	int i = mgr->mem->find(p);
	Node* n = mgr->mem->get(i);
	bytesAllocated -= n->t_size;
	if (n->type == Array)
		cout << "MEMMGR:delete type:variable\t# " << n->index << "\t @ " << p << "\tin" << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes !!!WRONG DELETE!!!" << endl;
	else
		cout << "MEMMGR:delete type:variable\t# " << n->index << "\t @ " << p << "\tin" << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes" << endl;
	mgr->mem->removeAt(i);

	free(p);
}
void log_del_a(void* p)
{
	int i = mgr->mem->find(p);
	Node* n = mgr->mem->get(i);
	bytesAllocated -= n->t_size;
	if (n->type == Variable)
		cout << "MEMMGR:delete type:array   \t# " << n->index << "\t @ " << p << "\tin " << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes !!!WRONG DELETE!!!" << endl;
	else
		cout << "MEMMGR:delete type:array   \t# " << n->index << "\t @ " << p << "\tin " << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes" << endl;
	mgr->mem->removeAt(i);
	free(p);
}

void init_mgr()
{
	cout << "MEMMGR:initializing" << endl;
	mgr = (MemMgr*)malloc(sizeof(MemMgr*));

	mgr->mem = (DLinkedList*)malloc(sizeof(DLinkedList*));
	mgr->mem->first = nullptr;
	mgr->mem->last = nullptr;

	atexit(finalize);
}

void log(void* addr, size_t size)
{
	Node* node = DLinkedList::create(addr, size, Variable, fileMem, lineMem);
	cout << "MEMMGR:new    type:variable\t# " << node->index << "\t @ " << addr << "\tin " << fileMem << ":" << lineMem << "\t of " << node->t_size << " bytes" << endl;
	mgr->mem->addLast(node);
}
void loga(void* addr, size_t size)
{
	Node* node = DLinkedList::create(addr, size, Array, fileMem, lineMem);
	cout << "MEMMGR:new    type:array   \t# " << node->index << "\t @ " << addr << "\tin " << fileMem << ":" << lineMem << "\t of " << node->t_size << " bytes" << endl;
	mgr->mem->addLast(node);
}

void finalize()
{
	cout << "+-----------------------+" << endl
		<< "| MemoryManager Summary |" << endl
		<< "+-----------------------+" << endl;

	if (bytesAllocated>maxBytesAllocated / 2)
		cout << "did you even try?" << endl;

	cout << endl << " > Number of leaks: " << mgr->mem->size() << endl
		<< " > Size of memory not deallocated: " << bytesAllocated << " bytes" << endl
		<< " > Maximum memory allocated on the heap: " << maxBytesAllocated << " bytes" << endl
		<< " > Leak summary:" << endl;
	mgr->mem->print();
}

extern myNew newFunc = init_new;
extern myNew newFuncA = init_new_a;
extern myDelete delFunc = log_del;
extern myDelete delFuncA = log_del_a;

void* operator new(size_t size) throw(std::bad_alloc)
{
	return newFunc(size);
}
void* operator new[](size_t size) throw(std::bad_alloc)
{
	return newFuncA(size);
}
void operator delete(void *p) throw()
{
	delFunc(p);
}
void operator delete[](void *p) throw()
{
	delFuncA(p);
}


bool hack(const char* file, int line)
{
	fileMem = file;
	lineMem = line;
	//cout<<file<<":"<<line<<endl;
	return false;
}

#define new hack(__FILE__,__LINE__)&&0?NULL:new
#define delete hack(__FILE__,__LINE__)&&0?NULL:delete
#endif
