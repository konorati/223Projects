#pragma once

// Implement this class in this header file

#include <iostream>
#include <queue>
#include <stack>

using namespace std;

// Notes about template object:
// Whatever object is used as the template type for this class must implement the following:
// - Copy constructor
// - int GetKey() function
// - string ToString() function

template <typename T>
class hw6_234tree
{
private:
	struct Node
	{
		// A, B and C are the actual data values in the node		
		T* A;
		T* B;
		T* C;

		// Child pointers
		Node *L, *M1, *M2, *R;

		// Constructor to initialize the node with one value. The other two value 
		// pointers will be set to null and all child pointers will be set to null. 
		// The pointer given as the parameter is directly copied to A.
		Node(T* valueForA)
		{
			A = valueForA;
			B = C = NULL;
			L = M1 = M2 = R = NULL;
		}

		// Constructor to initialize the node with one value and two child pointers.
		Node(T* valueForA, Node* leftChildOfA, Node* rightChildOfA)
		{
			A = valueForA;
			L = leftChildOfA;
			M1 = rightChildOfA;
			
			// Everything else is NULL
			B = C = NULL;
			M2 = R = NULL;
		}

		Node(T* aVal, T* bVal, T* cVal, Node* left, Node* m1, Node* m2, Node* right)
		{
			A = aVal;
			B = bVal;
			C = cVal;
			L = left;
			M1 = m1;
			M2 = m2;
			R = right;
		}

		bool ContainsKey(int key)
		{
			if (C && C->GetKey() == key) { return true; }
			if (B && B->GetKey() == key) { return true; }
			if (A->GetKey() == key) { return true; }
			return false;
		}

		void DeleteData()
		{
			if (A) { delete A; A = NULL; }
			if (B) { delete B; B = NULL; }
			if (C) { delete C; C = NULL; }
		}

		// Returns true if all three data-value pointers, A, B, and C, are non-null, 
		// false otherwise.
		bool IsFull()
		{
			return (A != NULL && B != NULL && C != NULL);
		}

		// Returns true if all 4 child pointers are null, implying that the node is a leaf
		bool IsLeaf()
		{
			return (NULL == L && NULL == M1 && NULL == M2 && NULL == R);
		}

		int ValueCount()
		{
			if (C) { return 3; }
			else if (B) { return 2; }
			else if (A) { return 1; }

			// Should never come here
			return 0;
		}
	};

	Node* m_root;

	void DeleteTree(Node* root)
	{
		if (root)
		{
			root->DeleteData();
			DeleteTree(root->L);
			DeleteTree(root->M1);
			DeleteTree(root->M2);
			DeleteTree(root->R);
			delete root;
		}
	}

	void Display(Node* n)
	{
		if (!n) { return; }
		
		// There will always be an A value with a subtree to left and right of it
		Display(n->L);
		string nodeAsString = n->A->ToString();
		cout << nodeAsString << endl;
		Display(n->M1);

		// There may or may not be a B and C value
		if (n->B)
		{
			nodeAsString = n->B->ToString();
			cout << nodeAsString << endl;
			Display(n->M2);

			if (n->C)
			{
				nodeAsString = n->C->ToString();
				cout << nodeAsString << endl;
				Display(n->R);
			}
		}
	}

	// Finds and returns the node containing the minimum value in the subtree.
	Node* FindMin(Node* root)
	{
		if (root->IsLeaf())
		{
			return root;
		}

		Node* n = root->L;
		while (n->L) { n = n->L; }
		return n;
	}
	// Finds and returns the node containing the maximum value in the subtree
	Node* FindMax(Node* root)
	{

		Node* n = root;
		while(!n->IsLeaf())
		{
			if(n->C)
			{
				n = n->R;
			}
			else if(n->B)
			{
				n = n->M2;
			}
			else
			{
				n = n->M1;
			}
		}
		return n;
	}

	#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid(Node* n)
	{
		if (!n) { return true; }

		// First make sure that A is non-null. This should always be true for any 
		// node in the tree.
		if (!n->A)
		{
			cout << "DEBUG: Tree error: Value A in node is null!" << endl;
			return false;
		}

		// Now make sure A < B < C within the node. If C is null then we just 
		// need to verify that A < B and if B and C are NULL then it's ok.
		if (n->C)
		{
			// B must not be NULL in this case
			if (NULL == n->B)
			{
				cout << "DEBUG: Tree error: Value C in node is non-null but B is null!" << endl;
				return false;
			}

			if (n->C->GetKey() <= n->B->GetKey())
			{
				cout << "DEBUG: Tree error: Value C in node is <= B!" << endl;
				return false;
			}
		}
		else if (n->B)
		{
			if (n->B->GetKey() <= n->A->GetKey())
			{
				cout << "DEBUG: Tree error: Value B in node is <= A!" << endl;
				return false;
			}
		}

		// Next we make sure we have non-null child pointers where appropriate.
		if (!n->IsLeaf())
		{
			if (n->C)
			{
				if (NULL == n->R)
				{
					cout << "DEBUG: Tree error: R pointer in node is NULL but value C is not!" << endl;
					return false;
				}
			}
			if (n->B)
			{
				if (NULL == n->M2)
				{
					cout << "DEBUG: Tree error: M2 pointer in node is NULL but value B is not!" << endl;
					return false;
				}
			}
			if (NULL == n->M1)
			{
				cout << "DEBUG: Tree error: M1 pointer in node (key=";
				cout << n->A->GetKey() << ") is NULL!" << endl;
				return false;
			}
			if (NULL == n->L)
			{
				cout << "DEBUG: Tree error: L pointer in node is NULL!" << endl;
				return false;
			}
		}

		// For each value in the node, the left subtree should be all less and the right 
		// subtree all greater.
		T* vals[] = {n->A, n->B, n->C};
		Node* children[] = {n->L, n->M1, n->M1, n->M2, n->M2, n->R};
		for (int i = 0; i < 3; i++)
		{
			if (NULL == vals[i]) { break; }
			int key = vals[i]->GetKey();

			// Make sure all keys in left subtree are less
			stack<Node*> nodes;
			nodes.push(children[i/2]);
			while (!nodes.empty())
			{
				Node* temp = nodes.top();
				nodes.pop();
				if (!temp) { continue; }

				if (temp->A->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->B && temp->B->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->C && temp->C->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}

				// Push children
				nodes.push(temp->L);
				nodes.push(temp->M1);
				nodes.push(temp->M2);
				nodes.push(temp->R);
			}

			// TODO: Right subtree - implement it on your own if you need the 
			// extra error checking
		}

		// Recurse for all children
		return (IsTreeValid(n->L) && IsTreeValid(n->M1) && 
			IsTreeValid(n->M2) && IsTreeValid(n->R));
	}
#endif

public:
	hw6_234tree(void)
	{
		m_root = NULL;
	}

	virtual ~hw6_234tree(void)
	{
		DeleteTree(m_root);
		m_root = NULL;
	}

	bool Add(T& toCopyAndAdd)
	{
		// You must implement this function
		// Do not allow duplicates in the tree
		// Return false if value already exists in tree or allocation of needed memory fails
		// Return true if addition succeeds
		// Don't forget the rules for splitting nodes
		
		if(Find(toCopyAndAdd.GetKey())) //If data value already exists in tree return false
		{
			return false;
		}
		//Allocate memory for new object
		T* obj = new T(toCopyAndAdd);
		if(obj == NULL) //If memory allocation failed return false
		{
			return false;
		}
		//Step 1: Find leaf to insert into
		Node *pCurrent = m_root;
		if(NULL == m_root) //Tree is empty
		{
			Node *newRoot = new Node(obj);
			m_root = newRoot;
			return true;
		}
		pCurrent = FindLeaf(obj);

		pCurrent = FindLeaf(obj);
		
		AddToLeaf(pCurrent, obj);
		
		
		if(m_root->C)
		{
			SplitRoot();
		}

		return true;
	}

	void Display(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty)";
		}
		Display(m_root);
	}

	void DisplayLevelKeys(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty, no levels to display)";
			return;
		}

		struct NodeLevel
		{
			Node* node;
			int level;
			NodeLevel(Node* n, int l) { node = n; level = l; }
		};
		
		queue<NodeLevel> nodes;
		nodes.push(NodeLevel(m_root, 0));
		int maxLevel = 0;
		while (!nodes.empty())
		{
			NodeLevel nl = nodes.front();
			nodes.pop();
			Node* n = nl.node;
			if (nl.level > maxLevel)
			{
				outstream << endl;
				maxLevel = nl.level;
			}

			outstream << "< " << n->A->GetKey() << " ";
			if (n->B) { outstream << n->B->GetKey() << " "; }
			if (n->C) { outstream << n->C->GetKey() << " "; }
			outstream << ">";

			// Enqueue children
			if (n->L) { nodes.push(NodeLevel(n->L, nl.level + 1)); }
			if (n->M1) { nodes.push(NodeLevel(n->M1, nl.level + 1)); }
			if (n->M2) { nodes.push(NodeLevel(n->M2, nl.level + 1)); }
			if (n->R) { nodes.push(NodeLevel(n->R, nl.level + 1)); }
		}
		outstream << endl;
	}

	T* Find(int key)
	{
		// You must implement this function to return the value for the object 
		// with the associated key. Return NULL if no such object exists in 
		// the tree.
		T* obj = NULL;
		Node *pCurrent = m_root;
		//While object not found and end of tree not reached continue traversing tree
		while(NULL == obj && NULL != pCurrent)
		{
			//Check Node A
			//If == A set obj to A
			if(pCurrent->A->GetKey() == key) //Key found
			{
				obj = pCurrent->A;
			}
			//Check which way to move
			else if(pCurrent->A->GetKey() > key) // If A is too big move to left child
			{
				pCurrent = pCurrent->L;
			}
			else //key must be greater than A
			{
				if(!(pCurrent->B)) //If Node has no more objects move to child M1
				{
					pCurrent = pCurrent->M1;
				}
				else
				{
					if(pCurrent->B->GetKey() == key) //Key found
					{
						obj = pCurrent->B;
					}
					else if(pCurrent->B->GetKey() > key) // If B is too big move to child M1
					{
						pCurrent = pCurrent->M1;
					}
					else //key must be greater than B
					{
						if(!(pCurrent->C)) //If Node has no more objects move to child M2
						{
							pCurrent = pCurrent->M2;
						}
						else
						{
							if(pCurrent->C->GetKey() == key) //Key found
							{
								obj = pCurrent->C;
							}
							else if(pCurrent->C->GetKey() > key) // If C is too big move to child M2
							{
								pCurrent = pCurrent->M2;
							}
							else //Key must be greater than C
							{
								pCurrent = pCurrent->R;
							}
						}
					}
				}
			}
		}
		return obj;
	}

	T* GetMin()
	{
		if (!m_root) { return NULL; }
		Node* n = FindMin(m_root);
		return n->A;
	}

	Node* FindParent(Node *child) // Finds parent node of given child (returns null if child is root);
	{
		if(child == m_root)
		{
			return NULL;
		}
		Node* parent = m_root;
		while(true)
		{
			if(child == parent->L || child == parent->M1 || child == parent->M2 || child == parent->R) //Parent is parent of child
			{
				return parent;
			}
			else
			{
				if(parent->A->GetKey() > child->A->GetKey()) //if parent > child move to child L
				{
					parent = parent->L;
				}
				else if(!parent->B || parent->B->GetKey() > child->A->GetKey()) //if no parent B or parent B > child move to child M1
				{
					parent = parent->M1;
				}
				else if(!parent->C || parent->C->GetKey() > child->A->GetKey()) //if no parent C or parent C > child move to child M2
				{
					parent = parent->M2;
				}
				else //Parent C < Child A
				{
					parent = parent->R;
				}
			}
		}
		return parent;
	}
	Node *FindLeaf(T* obj)
	{
		Node *pCurrent = m_root;
		Node *parent = NULL;
		if(m_root->C && !m_root->IsLeaf())
		{
			SplitRoot();
			pCurrent = m_root;
		}
		else if(m_root->C && m_root->IsLeaf())
		{
			SplitParent(m_root);
			pCurrent = m_root;
		}
		pCurrent = m_root;
		while(!pCurrent->IsLeaf()) // While not at a leaf continue to traverse tree
		{
			if(pCurrent->C)
			{
				if(pCurrent == m_root)
				{
					SplitRoot();
					pCurrent = m_root;
				}
				else
				{
					parent = FindParent(pCurrent);
					SplitNonParent(pCurrent, parent);
					pCurrent = parent;
				}
			}
			if(pCurrent->A->GetKey() > obj->GetKey()) //If A > obj move to left child
			{
				pCurrent = pCurrent->L;
			}
			else if(!pCurrent->B || pCurrent->B->GetKey() > obj->GetKey()) //If B does not exist or if B > obj move to child M1
			{
				pCurrent = pCurrent->M1;
			}
			else if(!pCurrent->C || pCurrent->C->GetKey() > obj->GetKey()) //If C does not exist or if C > obj move to child M2
			{
				pCurrent = pCurrent->M2;
			}
			else //Obj must be greater than C
			{
				pCurrent = pCurrent->R;
			}
		}
		if(pCurrent->C)
		{
			parent = FindParent(pCurrent);
			SplitNonParent(pCurrent, parent);
			pCurrent = parent;
			if(pCurrent->A->GetKey() > obj->GetKey()) //If A > obj move to left child
			{
				pCurrent = pCurrent->L;
			}
			else if(!pCurrent->B || pCurrent->B->GetKey() > obj->GetKey()) //If B does not exist or if B > obj move to child M1
			{
				pCurrent = pCurrent->M1;
			}
			else if(!pCurrent->C || pCurrent->C->GetKey() > obj->GetKey()) //If C does not exist or if C > obj move to child M2
			{
				pCurrent = pCurrent->M2;
			}
			else //Obj must be greater than C
			{
				pCurrent = pCurrent->R;
			}
		}
		return pCurrent;
	}

	Node *FindNode(T* obj)
	{
		Node *pCurrent = m_root;
		Node *pParent = NULL;
		Node *pSibling = NULL;
		bool found = false;
		while(!found)
		{
			pParent = pCurrent;
			pSibling = NULL;
			if(obj == pCurrent->A || obj == pCurrent->B || obj == pCurrent->C)
			{
				found = true;
			}
			else
			{
				if(pCurrent->A->GetKey() > obj->GetKey()) //If A > obj move to left child
				{
					pCurrent = pCurrent->L;
				}
				else if(!pCurrent->B || pCurrent->B->GetKey() > obj->GetKey()) //If B does not exist or if B > obj move to child M1
				{
					pCurrent = pCurrent->M1;
				}
				else if(!pCurrent->C || pCurrent->C->GetKey() > obj->GetKey()) //If C does not exist or if C > obj move to child M2
				{
					pCurrent = pCurrent->M2;
				}
				else //Obj must be greater than C
				{
					pCurrent = pCurrent->R;
				}
			}
			if(!pCurrent->B && !found) //If a single node, expand it
			{
				//Check to see if you can borrow from sibling
				// Check for siblings to borrow from
				if(pParent->L == pCurrent)
				{
					if(pParent->M1->B)
					{
						pSibling = pParent->M2;
					}
				}
				else if(pParent->M1 == pCurrent)
				{
					if(pParent->L->B)
					{
						pSibling = pParent->L;
					}
					else if(pParent->M2->B)
					{
						pSibling = pParent->M2;
					}
				}
				else if(pParent->M2 == pCurrent)
				{
					if(pParent->M1->B)
					{
						pSibling = pParent->M1;
					}
					else if(pParent->R->B)
					{
						pSibling = pParent->R;
					}
				}
				else if(pParent->R == pCurrent)
				{
					if(pParent->M2->B)
					{
						pSibling = pParent->M2;
					}
				}
				if(pSibling) //Sibling exists to borrow from so we rotate
				{
					Rotate(pCurrent, pParent, pSibling);
				}
				//Check to see if you can borrow from parent (fusion)
				 //Parents has more than one object so we can borrow & fuse //Note: Both pCurrent and pSibling will be single nodes
				else if(pParent->B)
				{
					BasicFusion(pCurrent, pParent);
				}
				else if(pParent == m_root) //Parent is root and has only 1 value and it's two children only have one value
				{
					if(pCurrent == pParent->L) //Current is left subtree
					{
						pSibling = pParent->M1;
						pParent->B = pParent->A;
						pParent->A = pCurrent->A;
						pParent->C = pSibling->A;
						pParent->L = pCurrent->L;
						pParent->M1 = pCurrent->M1;
						pParent->M2 = pSibling->L;
						pParent->R = pSibling->M1;
						Node *pTemp = pCurrent;
						pCurrent = m_root;
						delete pTemp;
						delete pSibling;
					}
					else //Current is right subtree
					{
						pSibling = pParent->L;
						pParent->B = pParent->A;
						pParent->A = pSibling->A;
						pParent->C = pCurrent->C;
						pParent->L = pSibling->L;
						pParent->M1 = pSibling->M1;
						pParent->M2 = pCurrent->L;
						pParent->R = pCurrent->M1;
						Node *pTemp = pCurrent;
						pCurrent = m_root;
						delete pTemp;
						delete pSibling;
					}
				}
			}
			

		}
		return pCurrent;
	}
	void BasicFusion(Node *pCurrent, Node *pParent)
	{
		Node *pSibling = NULL;
				if(pParent->L == pCurrent)
				{
					pSibling = pParent->M1;
					pCurrent->B = pParent->A;
					pParent->A = pParent->B;
					pParent->B = pParent->C;
					pParent->C = NULL;
					pParent->M1 = pParent->M2;
					pParent->M2 = pParent->R;
					pParent->R = NULL;

					pCurrent->C = pSibling->A;
					pCurrent->M2 = pSibling->L;
					pCurrent->R = pSibling->M1;
					delete pSibling;
				}
				else if(pParent->R == pCurrent)
				{
					pSibling = pParent->M2;
					pCurrent->C = pCurrent->A;
					pCurrent->B = pParent->C;
					pParent->C = NULL;
					pParent->M2 = pCurrent;
					pParent->R = NULL;
					pCurrent->A = pSibling->A;
					pCurrent->R = pCurrent->M1;
					pCurrent->M2 = pCurrent->L;
					pCurrent->M1 = pSibling->M1;
					pCurrent->L = pSibling->L;
					delete pSibling;
				}
				else if(pParent->M1 == pCurrent)
				{
					pSibling = pParent->L;
					pCurrent->C = pCurrent->A;
					pCurrent->B = pParent->A;
					pParent->L = pCurrent;
					pParent->M1 = pParent->M2;
					pParent->M2 = pParent->R;
					pParent->R = NULL;
					pParent->A = pParent->B;
					pParent->B = pParent->C;
					pParent->C = NULL;
					pCurrent->A = pSibling->A;
					pCurrent->R = pCurrent->M1;
					pCurrent->M2 = pCurrent->L;
					pCurrent->M1 = pSibling->M1;
					pCurrent->L = pSibling->L;
					delete pSibling;
				}
				else if(pParent->M2 == pCurrent)
				{
					pSibling = pParent->M1;
					pCurrent->C = pCurrent->A;
					pCurrent->B = pParent->B;
					pParent->B = pParent->C;
					pParent->C = NULL;
					pParent->M1 = pParent->M2;
					pParent->M2 = pParent->R;
					pParent->R = NULL;
					pCurrent->A = pSibling->A;
					pCurrent->R = pCurrent->M1;
					pCurrent->M2 = pCurrent->L;
					pCurrent->M1 = pSibling->M1;
					pCurrent->L = pSibling->L;
					delete pSibling;
				}
	}

	void AddToLeaf(Node *pCurrent, T* obj)
	{
		pCurrent->C = obj;
		if(!pCurrent->B || pCurrent->B->GetKey() > obj->GetKey()) //If there is no B or B > C swap B & C
		{
			T* temp = pCurrent->B;
			pCurrent->B = pCurrent->C;
			pCurrent->C = temp;
		}
		if(pCurrent->A->GetKey() > pCurrent->B->GetKey()) //If A > B swap A & B
		{
			T* temp = pCurrent->A;
			pCurrent->A = pCurrent->B;
			pCurrent->B = temp;
		}
	}
	void SplitParent(Node *child)
	{
		Node *newRoot = new Node(child->B);
		Node *newRight = new Node(child->C);

		//Set new root fields 
		newRoot->L = child;
		newRoot->M1 = newRight;

		//Set new right fields
		newRight->L = child->M2;
		newRight->M1 = child->R;

		//Remove obj from original root and set pointers to null
		child->B = NULL;
		child->C = NULL;
		child->M2 = NULL;
		child->R = NULL;

		//Set m_root to new root
		m_root = newRoot;
	}
	void SplitRoot()
	{
		Node *newRoot = new Node(m_root->B);
		Node *newRight = new Node(m_root->C);

		//Set new root fields
		newRoot->L = m_root;
		newRoot->M1 = newRight;

		//Set new Right fields
		newRight->L = m_root->M2;
		newRight->M1 = m_root->R;

		//Reset old root fields
		m_root->M2 = NULL;
		m_root->R = NULL;
		m_root->B = NULL;
		m_root->C = NULL;

		m_root = newRoot;
	}

	void SplitNonParent(Node *child, Node *parent)
	{
		Node *newRight = new Node(child->C, child->M2, child->R);
		AddToLeaf(parent, child->B);

		//Remove obj from original child and set pointers to null
		child->B = NULL;
		child->C = NULL;
		child->M2 = NULL;
		child->R = NULL;

		//Fix parent child pointers
		if(parent->M2 == child)
		{
			parent->R = newRight;
		}
		else if(parent->M1 == child)
		{
			if(!parent->M2)
			{
				parent->M2 = newRight;
			}
			else
			{
				parent->R = parent->M2;
				parent->M2 = newRight;
			}
		}
		else //Child must be left child of parent
		{
			if(parent->M2)
			{
				parent->R = parent->M2;
				parent->M2 = parent->M1;
				parent->M1 = newRight;
			}
			else
			{
				parent->M2 = parent->M1;
				parent->M1 = newRight;
			}
		}
		

	}

#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid()
	{
		return IsTreeValid(m_root);
	}
#endif

	bool Remove(int key)
	{
		if (!m_root) { return false; }

		// Implement the rest of this function for extra credit
		// Return false if the value is not present in the tree
		// Don't forget the rules for fusing nodes
		Node *pCurrent = NULL;
		Node *pReplace = NULL;
		Node *pSibling = NULL;
		Node *pParent = NULL;
		T* temp = NULL;
		T* obj = Find(key);
		if(obj)
		{
			pCurrent = FindNode(obj);
		}
		if(!pCurrent) //key not found
		{
			return false;
		}
		else
		{
			//Case 1: Easy - obj is a leaf with more than one key
			if(pCurrent->IsLeaf() && pCurrent->B)
			{
				DeleteT(pCurrent, obj);
				return true;
			}

			//Case 2: Medium - obj is a leaf with only one key
			if(pCurrent->IsLeaf() && !pCurrent->B && pCurrent != m_root)
			{
				pParent = FindParent(pCurrent);
				// Check for siblings to borrow from
				if(pParent->L == pCurrent)
				{
					if(pParent->M1->B)
					{
						pSibling = pParent->M2;
					}
				}
				else if(pParent->M1 == pCurrent)
				{
					if(pParent->L->B)
					{
						pSibling = pParent->L;
					}
					else if(pParent->M2->B)
					{
						pSibling = pParent->M2;
					}
				}
				else if(pParent->M2 == pCurrent)
				{
					if(pParent->M1->B)
					{
						pSibling = pParent->M1;
					}
					else if(pParent->R->B)
					{
						pSibling = pParent->R;
					}
				}
				else
				{
					if(pParent->M2->B)
					{
						pSibling = pParent->M2;
					}
				}
				// If pSibling is not NULL then a sibling to borrow from exists and a rotation will occur
				if(pSibling)
				{
					Rotate(pCurrent, pParent, pSibling);
					DeleteT(pCurrent, obj);
					return true;
				}
			}
			
			//Case 3: Medium - obj can be replaced with obj on a leaf(with more than one key)
			
			// Find the largest object on the left subtree
			if(!pCurrent->IsLeaf())
			{
				if(obj == pCurrent->A) 
				{
					pReplace = FindMax(pCurrent->L);
				}
				else if(obj == pCurrent->B)
				{
					pReplace = FindMax(pCurrent->M1);
				}
				else
				{
					pReplace = FindMax(pCurrent->M2);
				}
				if(pReplace->B) //If more than one object on leaf then we can swap the two objects
				{
					if(pReplace->C)
					{
						temp = pReplace->C;
						pReplace->C = obj;
					}
					else
					{
						temp = pReplace->B;
						pReplace->C = obj;
					}
					if(pCurrent->A == obj)
					{
						pCurrent->A = temp;
					}
					else if(pCurrent->B == obj)
					{
						pCurrent->B = temp;
					}
					else
					{
						pCurrent->C = obj;
					}
					//Delete obj (which is now in a leaf)
					DeleteT(pReplace, obj);
					return true;
				}
				else // Cannot use largest object on left subtree, so try smallest object on right subtree
				{
					if(obj == pCurrent->A)
					{
						pReplace = FindMin(pCurrent->M1);
					}
					else if(obj == pCurrent->B)
					{
						pReplace = FindMin(pCurrent->M2);
					}
					else
					{
						pReplace = FindMin(pCurrent->R);
					}
					
					if(pReplace->B) // We can use smallest object on right subtree
					{
						temp = pReplace->A;
						pReplace->A = obj;
						if(pCurrent->A == obj)
						{
							pCurrent->A = temp;
						}
						else if(pCurrent->B == obj)
						{
							pCurrent->B = temp;
						}
						else
						{
							pCurrent->C = temp;
						}
						//Delete obj (which is now in a leaf)
						DeleteT(pReplace, obj);
						return true;
					}
				}
			} // End case 3
			//Case 4: 
		}
		return true;
	}

	void Rotate(Node* current, Node* parent, Node* sibling)
	{
		T* temp = NULL;
		Node *tempChild = NULL;
		
		// If current key < sibling key Rotate Left
		if(current->A->GetKey() < sibling->A->GetKey())
		{
			temp = sibling->A;
			tempChild = sibling->L;
			sibling->A = sibling->B;
			sibling->B = sibling->C;
			sibling->C = NULL;
			sibling->L = sibling->M1;
			sibling->M1 = sibling->M2;
			sibling->M2 = sibling->R;
			sibling->R = NULL;

			if(current == parent->L)
			{
				current->B = parent->A;
				parent->A = temp;
			}
			else if(current == parent->M1)
			{
				current->B = parent->B;
				parent->B = temp;
			}
			else //current == parent->M2
			{
				current->B = parent->C;
				parent->C = temp;
			}
			current->M2 = tempChild;
		}
		// If current key > sibling key Rotate Right
		else
		{
			temp = sibling->C;
			tempChild = sibling->R;
			if(!temp)
			{
				temp = sibling->B;
				tempChild = sibling->M2;
				sibling->B = NULL;
				sibling->M2 = NULL;
			}
			sibling->C = NULL;
			sibling->R = NULL;
			current->B = current->A;

			if(current == parent->M1)
			{
				current->A = parent->A;
				parent->A = temp;
			}
			else if(current == parent->M2)
			{
				current->A = parent->B;
				parent->B = temp;
			}
			else //current == parent->R
			{
				current->A = parent->C;
				parent->C = temp;
			}
			current->M2 = current->M1;
			current->M1 = current->L;
			current->L = tempChild;
		}
	}

	void DeleteT(Node* current, T* obj)
	{
		T* temp = NULL;
		if(current->A == obj)
		{
			delete obj;
			current->A = current->B;
			current->B = current->C;
			current->C = NULL;
		}
		else if(current->B == obj)
		{
			delete obj;
			current->B = current->C;
			current->C = NULL;
		}
		else if(current->C == obj)
		{
			delete obj;
			current->C = NULL;
		}
	}
};

