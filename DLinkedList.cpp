#include "DLinkedList.h"
#include <cstdlib>
#include <iostream>

using namespace std;

DLinkedList::DLinkedList()
{
	first = nullptr; last = nullptr;
}

Node* DLinkedList::create(void* address, size_t size, AllocationType aType, const char* fileName, int lineNumber)
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

int DLinkedList::addFirst(Node* node)
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
int DLinkedList::addLast(Node* node)
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
int DLinkedList::addAt(Node* node, int i)
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
int DLinkedList::removeFirst()
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
int DLinkedList::removeLast()
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
int DLinkedList::removeAt(int i)
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
int DLinkedList::size()
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
Node* DLinkedList::get(int i)
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

int DLinkedList::find(void* address)
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

void DLinkedList::print()
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