// Homework 4
// Implement the Add and Remove functions for the AVL tree
// Remember the rules for each node in an AVL tree:
// 1. Entire left subtree is less (same as BST)
// 2. Entire right subtree is greater (same as BST)
// 3. abs(DepthOfLeftSubtree - DepthOfRightSubtree) <= 1

#include "HW4AVL.h"

using namespace std;

HW4AVL::Node::Node(int dataValue, Node* parent)
{
	Data = dataValue;
	Left = Right = NULL;
	Parent = parent;
}

// Deletes a child of this node and sets the appropriate child pointer to 
// NULL. The child must be the immediate left or right child of this node.
bool HW4AVL::Node::DeleteChild(Node* child)
{
	return ReplaceAndDeleteChild(child, NULL);
}

bool HW4AVL::Node::ReplaceAndDeleteChild(Node* child, Node* newChild)
{
	if (child == Left)
	{
		if (Left) { delete Left; }
		Left = newChild;
	}
	else if (child == Right)
	{
		if (Right) { delete Right; }
		Right = newChild;
	}
	else
	{
		// It's not a child of this node
		return false;
	}
	return true;
}

HW4AVL::HW4AVL(void)
{
	m_root = NULL;
}

HW4AVL::~HW4AVL(void)
{
	DeleteTree(m_root);
	m_root = NULL;
}

// Should return false if the value already exists in the tree or if memory 
// for a new node could not be allocated.
bool HW4AVL::Add(int dataValue)
{
	// Implement this function
	// Remember to balance before returning
	if (m_root)
	{
		Node* n = m_root;

		while (true)
		{
			if (dataValue == n->Data) { return false; }
			else if (dataValue < n->Data)
			{
				if (n->Left)
				{
					n = n->Left;
				}
				else
				{
					n->Left = new Node(dataValue, n);
					int lHeight = 0, rHeight = 0;
					if(n != NULL)
					{
						lHeight = Height(n->Left);
						rHeight = Height(n->Right);
					}
					while(n != NULL && (lHeight-rHeight) != 2 && (rHeight-lHeight) != 2)
					{
						n = n->Parent;
						if(n != NULL)
						{
							lHeight = Height(n->Left);
							rHeight = Height(n->Right);
						}
					}
					if(n != NULL) //Tree needs to be balanced
					{
						if(lHeight > rHeight)
						{
							if(dataValue > n->Left->Data)
							{
								rotateLeftTwice(&n);
							}
							else
							{
								rotateLeft(&n);
							}
						}
						else
						{
							if(dataValue < n->Right->Data)
							{
								rotateRightTwice(&n);
							}
							else
							{
								rotateRight(&n);
							}
						}
					}
					return true;
				}
			}
			else
			{
				if (n->Right)
				{
					n = n->Right;
				}
				else
				{
					n->Right = new Node(dataValue, n);
					int lHeight = 0, rHeight = 0;
					if(n != NULL)
					{
						lHeight = Height(n->Left);
						rHeight = Height(n->Right);
					}
					while(n != NULL && (lHeight-rHeight) != 2 && (rHeight-lHeight) != 2)
					{
						n = n->Parent;
						if(n != NULL)
						{
							lHeight = Height(n->Left);
							rHeight = Height(n->Right);
						}
					}
					if(n != NULL) //Tree needs to be balanced
					{
						if(lHeight > rHeight)
						{
							if(dataValue > n->Left->Data)
							{
								rotateLeftTwice(&n);
							}
							else
							{
								rotateLeft(&n);
							}
						}
						else
						{
							if(dataValue < n->Right->Data)
							{
								rotateRightTwice(&n);
							}
							else
							{
								rotateRight(&n);
							}
						}
					}
					return true;
				}
			}
		}
	}
	else
	{
		m_root = new Node(dataValue, NULL);
		return true;
	}
}

// Adds all nodes, including null ones, that reside on the specified level. The 
// level number is 0-based. Since null nodes are included, this function always 
// adds exactly 2^level nodes to the queue. This is intended to be a helpful 
// utility function for the tree displaying function, which wants information 
// about everything on a level so it can compute spacing for a nice display.
void HW4AVL::AddAllNodesOnLevel(int level, std::queue<Node*>& theQ)
{
	// Just return if level value is invalid
	if (level < 0)
	{
		return;
	}
	
	// Count the number of levels in the tree
	int lc = CountLevels();
	
	if (0 == level)
	{
		theQ.push(m_root);
	}
	else if (level >= lc)
	{
		// This means that we want nodes from a level that doesn't exist in the 
		// tree. So we just add 2^level null nodes to the queue.
		int count = 1 << level;
		while (count--)
		{
			theQ.push(NULL);
		}
	}
	else
	{
		// By no means is this the most efficient way to do this, but get the list of 
		// nodes for the previous level and just add their children.
		queue<Node*> prevLevel;
		AddAllNodesOnLevel(level - 1, prevLevel);

		while (!prevLevel.empty())
		{
			Node* parent = prevLevel.front();
			prevLevel.pop();

			// If the parent node is non-null, enqueue both children, otherwise enqueue 
			// 2 null children
			if (parent)
			{
				theQ.push(parent->Left);
				theQ.push(parent->Right);
			}
			else
			{
				theQ.push(NULL);
				theQ.push(NULL);
			}
		}
	}
}

// Utility function to compute the number of digits in the string representation of 
// the specified number in base-10.
int HW4AVL::CountDigits(int decimalValue)
{
	if (0 == decimalValue)
	{
		return 1;
	}

	int count = (decimalValue < 0) ? 1 : 0;
	while (decimalValue)
	{
		count++;
		decimalValue /= 10;
	}
	return count;
}

// Returns the number of levels in the tree. A tree with no contents (null root) will have 
// zero levels. A tree with just the root and no children has 1 level. For all other cases 
// the number of levels must be counted. There is a linear-time algorithm for this.
int HW4AVL::CountLevels()
{
	return CountLevels(m_root);
}

int HW4AVL::CountLevels(Node* n)
{
	if (NULL == n)
	{
		return 0;
	}

	int leftLevels = CountLevels(n->Left);
	int rightLevels = CountLevels(n->Right);
	return 1 + ((leftLevels > rightLevels) ? leftLevels : rightLevels);
}

void HW4AVL::DeleteTree(Node* root)
{
	if (root)
	{
		DeleteTree(root->Left);
		DeleteTree(root->Right);
		delete root;
	}
}

void HW4AVL::DisplayContents(Node* node, ostream& outputStream)
{
	/*
	if (node)
	{
		DisplayContents(node->Left, outputStream);
		
		// Display the numerical value for this node
		outputStream << node->Data << " ";
		
		// Display the entire right subtree
		DisplayContents(node->Right, outputStream);
	}
	*/

	// Non-recusive version
	stack<Node*> nodes;
	while (true)
	{
		while (true)
		{
			if (!node) { break; }
		
			if (node->Left)
			{
				nodes.push(node);
				node = node->Left;
				continue;
			}

			outputStream << node->Data << " ";
			node = node->Right;
		}
		if (nodes.empty()) { break; }
		else
		{
			node = nodes.top();
			nodes.pop();
			outputStream << node->Data << " ";
			node = node->Right;
		}
	}
}

void HW4AVL::DisplayContents(ostream& outputStream)
{
	outputStream << "BST Contents: ";
	DisplayContents(m_root, outputStream);
	outputStream << endl;
}

void HW4AVL::DisplayLevels(std::ostream& outputStream)
{
	if (NULL == m_root)
	{
		outputStream << "(tree is empty)" << endl;
		return;
	}

	int lc = CountLevels();
	outputStream << "Tree with " << lc << " levels:" << endl;

	struct NodeLevel
	{
		Node* N;
		int Level;

		NodeLevel(Node* n, int lvl)
		{
			N = n;
			Level = lvl;
		}
	};

	bool worst = false;
	bool secondWorst = false;

	if (worst) // Worst display mode
	{
		queue<NodeLevel> nodes;
		nodes.push(NodeLevel(m_root, 0));
		int maxlevel = 0;
		while (!nodes.empty())
		{
			NodeLevel nl = nodes.front();
			nodes.pop();

			// See if we've hit a new level
			if (nl.Level > maxlevel)
			{
				outputStream << endl;
				maxlevel = nl.Level;
			}

			if (nl.N)
			{
				// Display the node's data and then enqueue children
				outputStream << nl.N->Data << " ";
				nodes.push(NodeLevel(nl.N->Left, nl.Level + 1));
				nodes.push(NodeLevel(nl.N->Right, nl.Level + 1));
			}
		}
	}
	else if (secondWorst) // Second worst display mode
	{
		for (int i = 0; i < CountLevels(); i++)
		{
			queue<Node*> theQ;
			AddAllNodesOnLevel(i, theQ);
			while (!theQ.empty())
			{
				Node* temp = theQ.front();
				theQ.pop();
				if (temp)
				{
					outputStream << temp->Data << " ";
				}
				else
				{
					outputStream << "{0} ";
				}
			}

			outputStream << endl;
		}
	}
	else // Smart display mode
	{
		// First find the maximum value in the tree and compute the number of digits
		// Note that we're assuming all positive numbers
		int max = GetMax();
		int charsPerNum = CountDigits(max);
		// We want at least one space after each number but also since rounding stuff 
		// gets weird we want a little extra padding
		charsPerNum+=2;
		// We display null values as "{0} " so we need at least 4 digits
		if (charsPerNum < 4) { charsPerNum = 4; }

		// Count the levels in the tree and then calculate how many characters will be 
		// needed to display the bottommost level.
		int lc = CountLevels();
		int charsOnLast = charsPerNum * (1 << (lc - 1));
		
		// Now loop through the rows and display them
		for (int i = 0; i < lc; i++)
		{
			queue<Node*> theQ;
			AddAllNodesOnLevel(i, theQ);

			float spacesBetweenNumCenters = (float)charsOnLast / (float)theQ.size();
			float numCenter = spacesBetweenNumCenters / 2.0f;
			int charAt = 0;
			
			while (!theQ.empty())
			{
				Node* temp = theQ.front();
				theQ.pop();
				int digitsForThisOne;
				if (temp)
				{
					digitsForThisOne = CountDigits(temp->Data);
				}
				else
				{
					digitsForThisOne = 3;
				}

				// Put right amount of spacing before this symbol
				int spaceCount = (int)(numCenter - (float)charAt - ((float)digitsForThisOne / 2.0f));
				if (spaceCount < 0)
				{
					bool breakhere = true;
				}
				if (spaceCount < 1) { spaceCount = 1; }
				DisplaySpaces(spaceCount, outputStream);

				// Display the number or symbol and update charAt and numCenter
				if (temp)
				{
					outputStream << temp->Data;
				}
				else
				{
					outputStream << "{0}";
				}
				charAt += spaceCount + digitsForThisOne;
				numCenter += spacesBetweenNumCenters;
			}

			outputStream << endl;
		}
	}
}

void HW4AVL::DisplaySpaces(int spaceCount, ostream& outputStream)
{
	while (spaceCount--)
	{
		outputStream << " ";
	}
}

void HW4AVL::rotateLeft(Node **node)
{
	Node *pTemp;
	pTemp = (*node)->Left;
	(*node)->Left = pTemp->Right;
	if(pTemp->Right != NULL)
	{
		pTemp->Right->Parent = (*node);
	}
	pTemp->Right = *node;
	pTemp->Parent = (*node)->Parent;
	(*node)->Parent = pTemp;
	*node = pTemp;
	if((*node)->Parent == NULL)
	{
		m_root = *node;
	}
	else
	{
		if((*node)->Parent->Data < (*node)->Data)
		{
			(*node)->Parent->Right = *node;
		}
		else
		{
			(*node)->Parent->Left = *node;
		}
	}
}

void HW4AVL::rotateRight(Node **node)
{
	Node *pTemp;
	pTemp = (*node)->Right;
	(*node)->Right = pTemp->Left;
	if(pTemp->Left != NULL)
	{
		pTemp->Left->Parent = (*node);
	}
	pTemp->Left = *node;
	pTemp->Parent = (*node)->Parent;
	(*node)->Parent = pTemp;
	*node = pTemp;
	if((*node)->Parent == NULL)
	{
		m_root = *node;
	}
	else
	{
		if((*node)->Parent->Data < (*node)->Data)
		{
			(*node)->Parent->Right = *node;
		}
		else
		{
			(*node)->Parent->Left = *node;
		}
	}
}
	
void HW4AVL::rotateLeftTwice(Node **node)
{
	rotateRight(&((*node)->Left));
	rotateLeft(node);
}

void HW4AVL::rotateRightTwice(Node **node)
{
	rotateLeft(&((*node)->Right));
	rotateRight(node);
}

int HW4AVL::Height(Node *node)
{
	int left, right;

	if(NULL == node)
	{
		return 0;
	}
	left = Height(node->Left);
	right = Height(node->Right);
	if(left > right)
	{
		return left+1;
	}
	else
	{
		return right+1;
	}
}

// Gets the maximum value in the tree. Returns 0 if the tree is empty.
int HW4AVL::GetMax()
{
	if (m_root)
	{
		Node* n = m_root;
		while (n->Right)
		{
			n = n->Right;
		}
		return n->Data;
	}
	return 0;
}

bool HW4AVL::IsEmpty()
{
	return (NULL == m_root);
}

// Returns true if the value was found and removed, false if it was not found
bool HW4AVL::Remove(int dataValue)
{
	if (!m_root)
	{
		return false;
	}

	// Implement (the rest of) this function
	// Remember to balance before returning
		Node* parent = NULL;
	Node* deleteMe = m_root;
	while (true)
	{
		if (deleteMe->Data == dataValue)
		{
			break;
		}

		else if (dataValue > deleteMe->Data)
		{
			if (deleteMe->Right)
			{
				parent = deleteMe;
				deleteMe = deleteMe->Right;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (deleteMe->Left)
			{
				parent = deleteMe;
				deleteMe = deleteMe->Left;
			}
			else
			{
				return false;
			}
		}
	}

	// We have the node we want to delete and its parent

	// Case 1: It's a leaf
	if (NULL == deleteMe->Left && NULL == deleteMe->Right)
	{
		if (parent)
		{
			parent->DeleteChild(deleteMe);
		}
		else
		{
			delete m_root;
			m_root = NULL;
		}

	}

	// Case 2a: Null left child, non-null right
	else if (NULL == deleteMe->Left)
	{
		if (parent)
		{
			parent->ReplaceAndDeleteChild(deleteMe, deleteMe->Right);
		}
		else
		{
			m_root = m_root->Right;
			delete deleteMe;
		}
	}

	// Case 2b: Null right child, non-null left
	else if (NULL == deleteMe->Right)
	{
		if (parent)
		{
			parent->ReplaceAndDeleteChild(deleteMe, deleteMe->Left);
		}
		else
		{
			m_root = m_root->Left;
			delete deleteMe;
		}
	}

	// Case 3: Non-null left and right children
	else
	{
		// (Arbitrary) Find the min in the right subtree
		Node* min = deleteMe->Right;
		while (min->Left)
		{
			min = min->Left;
		}

		int minValue = min->Data;
		Remove(minValue);
		deleteMe->Data = minValue;
		return true;
	}
	// Check for balance & Balance if necessary
	int lHeight = 0, rHeight = 0;
	if(parent != NULL)
	{
		lHeight = Height(parent->Left);
		rHeight = Height(parent->Right);
	}
	while(parent != NULL && (lHeight-rHeight) != 2 && (rHeight-lHeight) != 2)
	{
		parent = parent->Parent;
		if(parent != NULL)
		{
			lHeight = Height(parent->Left);
			rHeight = Height(parent->Right);
		}
	}
	if(parent != NULL) //Need to balance
	{
		Node* child = NULL;
		if(Height(parent->Left) > Height(parent->Right))
		{
			child = parent->Left;
		}
		else
		{
			child = parent->Right;
		}
		Node* grandChild = NULL;
		if(Height(child->Left) > Height(child->Right))
		{
			grandChild = child->Left;
		}
		else
		{
			grandChild = child->Right;
		}

		if(child->Data < parent->Data)
		{
			if(grandChild->Data < child->Data)
			{
				rotateLeft(&parent);
			}
			else
			{
				rotateLeftTwice(&parent);
			}
		}
		else
		{
			if(grandChild->Data > child->Data)
			{
				rotateRight(&parent);
			}
			else
			{
				rotateRightTwice(&parent);
			}
		}
	}
	return true;
}


