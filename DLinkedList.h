#pragma once
#include "DLinkedListNode.h"

struct DLinkedList
{
	Node* first;
	Node* last;

	DLinkedList::DLinkedList();

	static Node* DLinkedList::create(void* address, size_t size, AllocationType aType, const char* fileName, int lineNumber);
	
	int DLinkedList::addFirst(Node* node);
	int DLinkedList::addLast(Node* node);
	int DLinkedList::addAt(Node* node, int i);

	int DLinkedList::removeFirst();
	int DLinkedList::removeLast();
	int DLinkedList::removeAt(int i);

	int DLinkedList::size();
	Node *DLinkedList::get(int i);
	int DLinkedList::find(void *address);
	void DLinkedList::print();
};
