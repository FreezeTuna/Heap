#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

enum TreeOrder
{
	Preorder,
	Inorder,
	Postorder,
	Levelorder,
	Max
};

template<typename T>
class HeapTree
{
public:
	struct HeapNode;
	struct ListNode;

public:
	static HeapNode* Create(T** InData)
	{
		HeapNode* node = new HeapNode();
		node->Data = *InData;
		node->Left = NULL;
		node->Right = NULL;

		return node;
	}

public:
	void InsertNode(HeapNode* InNode)
	{
		if (m_RootNode == NULL)
		{
			m_RootNode = InNode;
			InsertList(InNode, &m_LastNodeList);
			return;
		}

		GetInsertLocation(InNode);
		InsertList(InNode, &m_LastNodeList);
		ReplaceNode(GetLastHeapNode());
	}

	void PopNode()
	{
		if (m_RootNode == NULL)
		{
			return;
		}
	}

	void RemoveAllNode()
	{
		if (m_RootNode == NULL)
		{
			return;
		}
	}

	void PrintTree(TreeOrder InOrder)
	{
		if (m_RootNode == NULL)
		{
			return;
		}

		switch (InOrder)
		{
		case TreeOrder::Preorder:
			cout << "Preorder!" << endl;
			PrintPreorder(m_RootNode);
			break;

		case TreeOrder::Inorder:
			cout << "Inorder!" << endl;
			PrintInorder(m_RootNode);
			break;

		case TreeOrder::Postorder:
			cout << "Postorder!" << endl;
			PrintPostorder(m_RootNode);
			break;

		default:
			break;
		}

		cout << endl;
	}

private:
	void InsertList(HeapNode* InHeapNode, ListNode** InListNode = NULL)
	{
		if (*InListNode == NULL)
		{
			*InListNode = new ListNode();
			(*InListNode)->Node = InHeapNode;
			(*InListNode)->Next = NULL;

			return;
		}

		if ((*InListNode)->Next == NULL)
		{
			ListNode* listNode = new ListNode();
			listNode->Node = InHeapNode;
			listNode->Next = NULL;
			(*InListNode)->Next = listNode;
			return;
		}
		else
		{
			InsertList(InHeapNode, &(*InListNode)->Next);
		}
	}

	void ChangeList(HeapNode* InHeapParent, HeapNode* InHeapCur, ListNode** InListNode)
	{
		if (*InListNode == NULL)
		{
			return;
		}

		if ((*InListNode)->Node == InHeapParent)
		{
			(*InListNode)->Node = InHeapCur;
		}

		ChangeList(InHeapParent, InHeapCur, &(*InListNode)->Next);
	}

	void GetInsertLocation(HeapNode* InNode)
	{
		if (m_ParentNodeList == NULL)
		{
			m_ParentNodeList = m_LastNodeList;
			m_LastNodeList = NULL;
		}

		if (m_ParentNodeList != NULL)
		{
			if (m_ParentNodeList->Node->Left == NULL)
			{
				m_ParentNodeList->Node->Left = InNode;
				InNode->Parent = m_ParentNodeList->Node;
			}
			else if (m_ParentNodeList->Node->Right == NULL)
			{
				m_ParentNodeList->Node->Right = InNode;
				InNode->Parent = m_ParentNodeList->Node;
			}
			else
			{
				m_ParentNodeList = m_ParentNodeList->Next;
				GetInsertLocation(InNode);
			}
		}
	}

	HeapNode* GetLastHeapNode()
	{
		if (m_LastNodeList == NULL)
		{
			return NULL;
		}

		ListNode* listNode = m_LastNodeList;
		while (listNode->Next != NULL)
		{
			listNode = listNode->Next;
		}

		return listNode->Node;
	}

private:
	void ReplaceNode(HeapNode* InNode)
	{
		if (InNode == NULL || InNode->Parent == NULL)
		{
			return;
		}

		int result = GetDataOrder(InNode->Parent, InNode->Data);

		if (result == -1)
		{
			return;
		}

		HeapNode* temp = InNode->Parent;
		if (InNode->Parent->Parent != NULL)
		{
			if (InNode->Parent->Parent->Left == InNode->Parent)
			{
				InNode->Parent->Parent->Left = InNode;
			}
			else if (InNode->Parent->Parent->Right == InNode->Parent)
			{
				InNode->Parent->Parent->Right = InNode;
			}

			InNode->Parent = InNode->Parent->Parent;
		}
		else
		{
			m_RootNode = InNode;
			InNode->Parent = NULL;
		}


		HeapNode* inNodeLeft = InNode->Left;
		HeapNode* inNodeRight = InNode->Right;

		if (inNodeLeft == NULL && inNodeRight == NULL)
		{
			ChangeList(temp, InNode, &m_ParentNodeList);
			ChangeList(InNode, temp, &m_LastNodeList);
		}
		else
		{
			ChangeList(InNode, temp, &m_ParentNodeList);
		}

		if (temp->Left != InNode)
		{
			InNode->Left = temp->Left;
			InNode->Right = temp;
		}
		else
		{
			InNode->Left = temp;
			InNode->Right = temp->Right;
		}

		if (InNode->Left != NULL)
		{
			InNode->Left->Parent = InNode;
		}

		if (InNode->Right != NULL)
		{
			InNode->Right->Parent = InNode;
		}

		temp->Left = inNodeLeft;
		temp->Right = inNodeRight;

		if (temp->Left != NULL)
		{
			temp->Left->Parent = temp;
		}		
		
		if (temp->Right != NULL)
		{
			temp->Right->Parent = temp;
		}

		ReplaceNode(InNode);
	}

private:
	int IntDataOrder(T* RootData, T* InsertData)
	{
		if (*RootData == *InsertData)
		{
			return 0;
		}
		else if (*RootData < *InsertData)
		{
			return -1;
		}
		else if (*RootData > *InsertData)
		{
			return 1;
		}
	}

	int StringDataOrder(T* RootData, T* InsertData)
	{
		stringstream RootString;
		stringstream InsertString;

		RootString << *RootData;
		InsertString << *InsertData;

		if (RootString.str().compare(InsertString.str()) > 0)
		{
			return 1;
		}
		else if (RootString.str().compare(InsertString.str()) < 0)
		{
			return -1;
		}

		return 0;
	}

	int GetDataOrder(HeapNode* InRoot, T* InData)
	{
		string name = typeid(*InData).name();
		int result = -99999;
		if (name.find("int") != string::npos)
		{
			result = IntDataOrder(InRoot->Data, InData);
		}
		else if (name.find("string") != string::npos)
		{
			result = StringDataOrder(InRoot->Data, InData);
		}

		return result;
	}

private:
	void PrintPreorder(HeapNode* InNode)
	{
		if (InNode == NULL)
		{
			return;
		}

		cout << *InNode->Data << " -> ";
		PrintPreorder(InNode->Left);
		PrintPreorder(InNode->Right);
	}

	void PrintInorder(HeapNode* InNode = NULL)
	{
		if (InNode == NULL)
		{
			return;
		}

		PrintInorder(InNode->Left);
		cout << *InNode->Data << " -> ";
		PrintInorder(InNode->Right);
	}

	void PrintPostorder(HeapNode* InNode = NULL)
	{
		if (InNode == NULL)
		{
			return;
		}

		PrintPostorder(InNode->Left);
		PrintPostorder(InNode->Right);
		cout << *InNode->Data << " -> ";
	}

public:
	struct HeapNode
	{
		T* Data;
		HeapNode* Parent;
		HeapNode* Left;
		HeapNode* Right;
	};

	struct ListNode
	{
		HeapNode* Node;
		ListNode* Next;
	};

private:
	HeapNode* m_RootNode = NULL;

private:
	ListNode* m_ParentNodeList = NULL;
	ListNode* m_LastNodeList = NULL;
};