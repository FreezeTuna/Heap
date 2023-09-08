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

private:
	int IntDataOrder(T* RootData, T* InsertData)
	{
		if (*RootData < *InsertData)
		{
			return -1;
		}
		else if (*RootData > *InsertData)
		{
			return 1;
		}

		return 0;
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
	HeapNode* m_LastNode = NULL;

private:
	ListNode* m_ParentNodeList = NULL;
	ListNode* m_LastNodeList = NULL;
};