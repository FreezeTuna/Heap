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

	cout << "end" << endl;
}