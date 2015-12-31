#pragma once

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

	/*Node();
	Node(void* memoryAddress, size_t size);*/
};
