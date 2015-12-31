#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)
#include "MemMgr.h"
#include "DLinkedListNode.h"
#include "DLinkedList.h"



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
		cout << "MEMMGR:delete type:variable\t# " << n->index << "\t @ " << p << "\tin " << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes !!!WRONG DELETE!!!" << endl;
	else
		cout << "MEMMGR:delete type:variable\t# " << n->index << "\t @ " << p << "\tin " << fileMem << ":" << lineMem << "\t of " << n->t_size << " bytes" << endl;
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
	mgr = static_cast<MemMgr*>(malloc(sizeof(MemMgr*)));

	mgr->mem = static_cast<DLinkedList*>(malloc(sizeof(DLinkedList*)));
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

bool hack(const char* file, int line)
{
	fileMem = file;
	lineMem = line;
	//cout << file << ":" << line << endl;
	return false;
}

#endif