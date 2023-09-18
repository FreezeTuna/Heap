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
		ReplaceInsertNode(GetLastHeapNode());
	}

	HeapNode* PopNode()
	{
		if (m_RootNode == NULL)
		{
			return NULL;
		}

		HeapNode* popNode = m_RootNode;
		HeapNode* lastNode = GetLastHeapNode();

		DeleteList(lastNode, &m_LastNodeList);

		if (popNode == lastNode)
		{
			m_RootNode = NULL;
			return popNode;
		}
		else
		{
			m_RootNode = lastNode;
		}

		if (lastNode->Parent->Left == lastNode)
		{
			lastNode->Parent->Left = NULL;
		}
		else if (lastNode->Parent->Right == lastNode)
		{
			lastNode->Parent->Right = NULL;
		}

		lastNode->Parent = NULL;
		lastNode->Left = popNode->Left;
		lastNode->Right = popNode->Right;

		if (lastNode->Left != NULL)
		{
			lastNode->Left->Parent = lastNode;
		}

		if (lastNode->Right != NULL)
		{
			lastNode->Right->Parent = lastNode;
		}

		popNode->Left = NULL;
		popNode->Right = NULL;


		ReplacePopNode(m_RootNode);

		m_ParentNodeList = NULL;
		SettingParentList(m_RootNode);

		if (m_LastNodeList == NULL)
		{
			SettingLastList(m_RootNode);
		}

		return popNode;
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

	void ChangeList(HeapNode* InChangeNode, HeapNode* InHeapCur, ListNode** InListNode)
	{
		if (*InListNode == NULL)
		{
			return;
		}

		if ((*InListNode)->Node == InChangeNode)
		{
			(*InListNode)->Node = InHeapCur;
			return;
		}

		ChangeList(InChangeNode, InHeapCur, &(*InListNode)->Next);
	}

	void DeleteList(HeapNode* InDeleteNode, ListNode** InListNode)
	{
		if (*InListNode == NULL)
		{
			return;
		}

		if ((*InListNode)->Node == InDeleteNode)
		{
			(*InListNode) = NULL;
			return;
		}

		DeleteList(InDeleteNode, &(*InListNode)->Next);
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

	void SettingParentList(HeapNode* InNode)
	{
		if (InNode == NULL)
		{
			return;
		}

		if (InNode->Right == NULL)
		{
			if (IsInLastList(InNode, m_LastNodeList) == false)
			{
				InsertList(InNode, &m_ParentNodeList);
				return;
			}
		}

		SettingParentList(InNode->Left);
		SettingParentList(InNode->Right);
	}

	void SettingLastList(HeapNode* InNode)
	{
		if (InNode == NULL)
		{
			return;
		}

		if (InNode->Left == NULL)
		{
			InsertList(InNode, &m_LastNodeList);
			return;
		}

		SettingLastList(InNode->Left);
		SettingLastList(InNode->Right);
	}

	bool IsInLastList(HeapNode* InCompare, ListNode* InListNode)
	{
		if (InListNode == NULL)
		{
			return false;
		}

		if (InListNode->Node == InCompare)
		{
			return true;
		}

		IsInLastList(InCompare, InListNode->Next);
	}

private:
	void ReplaceInsertNode(HeapNode* InNode)
	{
		if (InNode == NULL || InNode->Parent == NULL)
		{
			return;
		}

		int result = GetDataOrder(InNode->Parent, InNode);

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

		ReplaceInsertNode(InNode);
	}

	void ReplacePopNode(HeapNode* InNode)
	{
		if (InNode == NULL || (InNode->Left == NULL && InNode->Right == NULL))
		{
			return;
		}

		int result = 0;

		if (InNode->Right != NULL)
		{
			result = GetDataOrder(InNode->Left, InNode->Right);
		}
		else
		{
			result = GetDataOrder(InNode, InNode->Left);

			if (result == 1)
			{
				result = -1;
			}
		}
		
		//	Left
		if(result == -1)
		{
			result = GetDataOrder(InNode, InNode->Left);
			if (result != 1)
			{
				return;
			}

			if (InNode->Parent == NULL)
			{
				m_RootNode = InNode->Left;
				InNode->Left->Parent = NULL;
			}

			HeapNode* inNodeLeft = InNode->Left;
			HeapNode* inNodeRight = InNode->Right;
			HeapNode* inNodeParent = InNode->Parent;

			if (inNodeLeft->Left == NULL && inNodeLeft->Right == NULL)
			{
				ChangeList(InNode, inNodeLeft, &m_ParentNodeList);
				ChangeList(inNodeLeft, InNode, &m_LastNodeList);
			}
			else
			{
				ChangeList(inNodeLeft, InNode, &m_ParentNodeList);
			}

			InNode->Left = inNodeLeft->Left;
			InNode->Right = inNodeLeft->Right;
			InNode->Parent = inNodeLeft;

			inNodeLeft->Left = InNode;
			inNodeLeft->Right = inNodeRight;

			if (inNodeRight != NULL)
			{
				inNodeRight->Parent = inNodeLeft;
			}

			inNodeLeft->Parent = inNodeParent;

			if (inNodeParent != NULL)
			{
				if (inNodeParent->Left == InNode)
				{
					inNodeParent->Left = inNodeLeft;
				}
				else if (inNodeParent->Right == InNode)
				{
					inNodeParent->Right = inNodeLeft;
				}
			}
		}
		//	Right
		else if (result == 1)
		{
			result = GetDataOrder(InNode, InNode->Right);
			if (result != 1)
			{
				return;
			}

			if (InNode->Parent == NULL)
			{
				m_RootNode = InNode->Right;
				InNode->Right->Parent = NULL;
			}

			HeapNode* inNodeLeft = InNode->Left;
			HeapNode* inNodeRight = InNode->Right;
			HeapNode* inNodeParent = InNode->Parent;

			if (inNodeRight->Left == NULL && inNodeRight->Right == NULL)
			{
				ChangeList(InNode, inNodeRight, &m_ParentNodeList);
				ChangeList(inNodeRight, InNode, &m_LastNodeList);
			}
			else
			{
				ChangeList(inNodeRight, InNode, &m_ParentNodeList);
			}

			InNode->Left = inNodeRight->Left;
			InNode->Right = inNodeRight->Right;
			InNode->Parent = inNodeRight;

			inNodeRight->Left = inNodeLeft;
			inNodeRight->Right = InNode;
			inNodeRight->Parent = inNodeParent;
			inNodeLeft->Parent = inNodeRight;
			
			if (inNodeParent != NULL)
			{
				if (inNodeParent->Left == InNode)
				{
					inNodeParent->Left = inNodeRight;
				}
				else if (inNodeParent->Right == InNode)
				{
					inNodeParent->Right = inNodeRight;
				}
			}
		}
		else
		{
			return;
		}


		if (InNode->Left != NULL)
		{
			InNode->Left->Parent = InNode;
		}

		if (InNode->Right != NULL)
		{
			InNode->Right->Parent = InNode;
		}

		ReplacePopNode(InNode);
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

	int GetDataOrder(HeapNode* InRoot, HeapNode* CompareNode)
	{
		string name = typeid(*(CompareNode->Data)).name();
		int result = -99999;
		if (name.find("int") != string::npos)
		{
			result = IntDataOrder(InRoot->Data, CompareNode->Data);
		}
		else if (name.find("string") != string::npos)
		{
			result = StringDataOrder(InRoot->Data, CompareNode->Data);
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