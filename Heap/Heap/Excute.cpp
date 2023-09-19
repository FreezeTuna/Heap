#include "Heap.h"

typedef HeapTree<int> Heap;

int main()
{
	Heap* heap = new Heap;

	for (int i = 15; i >= 1; i--)
	{
		int* index = new int;
		*index = *(&i);
		heap->InsertNode(Heap::Create(&index));
		/*string* name = new string;
		*name = "string";
		bTree->InsertNode(BTree::Create(&name));*/
	}

	heap->PrintTree(TreeOrder::Preorder);

	for (int i = 1; i <= 7; i++)
	{
		Heap::HeapNode* node = heap->PopNode();
		cout << *node->Data << endl;
		//heap->PrintTree(TreeOrder::Preorder);
	}

	for (int i = 2; i <= 7; i++)
	{
		int* index = new int;
		*index = *(&i);
		heap->InsertNode(Heap::Create(&index));
		/*string* name = new string;
		*name = "string";
		bTree->InsertNode(BTree::Create(&name));*/
	}

	Heap::HeapNode* node = heap->PopNode();
	cout << *node->Data << endl;
	heap->PrintTree(TreeOrder::Preorder);

	int* index = new int;
	*index = 20;
	heap->InsertNode(Heap::Create(&index));

	node = heap->PopNode();
	cout << *node->Data << endl;
	heap->PrintTree(TreeOrder::Preorder);

	index = new int;
	*index = 20;
	heap->InsertNode(Heap::Create(&index));

	node = heap->PopNode();
	cout << *node->Data << endl;
	heap->PrintTree(TreeOrder::Preorder);

	index = new int;
	*index = 20;
	heap->InsertNode(Heap::Create(&index));

	for (int i = 1; i <= 14; i++)
	{
		/*Heap::HeapNode**/ node = heap->PopNode();
		if (node != NULL)
		{
			cout << *node->Data << endl;
		}
		heap->PrintTree(TreeOrder::Preorder);
	}

	cout << "end" << endl;
}