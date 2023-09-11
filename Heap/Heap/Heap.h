#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

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
			InsertList(InNode, m_LastNodeList);
			return;
		}

		HeapNode* lastNode = NULL;
		int result = GetInsertLocation(lastNode);
		if (result == -1)
		{
			lastNode->Left = InNode;
		}
		else if (result == 1)
		{
			lastNode->Right = InNode;
		}
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

private:
	void InsertList(HeapNode* InHeapNode, ListNode* InListNode = NULL)
	{
		if (InListNode == NULL)
		{
			ListNode* listNode = new ListNode();
			listNode->Node = InHeapNode;
			listNode->Next = NULL;
			InListNode = listNode;

			return;
		}

		if (InListNode->Next == NULL)
		{
			ListNode* listNode = new ListNode();
			listNode->Node = InHeapNode;
			listNode->Next = NULL;
			InListNode->Next = listNode;
			return;
		}
		else
		{
			InsertList(InHeapNode, InListNode->Next);
		}
	}

	int GetInsertLocation(HeapNode& InNode)
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
				InNode = m_ParentNodeList->Node;
				return -1;
			}
			else if (m_ParentNodeList->Node->Right == NULL)
			{
				InNode = m_ParentNodeList->Node;
				return 1;
			}
			else
			{
				m_ParentNodeList = m_ParentNodeList->Next;
				GetInsertLocation(InNode);
			}
		}

		return -99;
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

public:
	struct HeapNode
	{
		T* Data;
		HeapNode* Left;
		HeapNode* Right;
	};

	struct ListNode
	{
		HeapNode* Node;
		ListNode* NextNode;
	};

private:
	HeapNode* m_RootNode = NULL;

private:
	ListNode* m_ParentNodeList = NULL;
	ListNode* m_LastNodeList = NULL;
};