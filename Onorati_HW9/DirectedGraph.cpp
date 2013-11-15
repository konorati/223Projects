#include "DirectedGraph.h"

DirectedGraph::DirectedGraph()
{
	// No initialization needed
}

DirectedGraph::~DirectedGraph()
{
	// Free all nodes and edges
	for (int i = 0; i < m_nodes.size(); i++)
	{
		delete m_nodes[i];
	}
	m_nodes.clear();
	for (int i = 0; i < m_edges.size(); i++)
	{
		delete m_edges[i];
	}
	m_edges.clear();

	// Free/clear additional things here if needed
}

bool DirectedGraph::AddNode(const string& nodeName, const string& nodeData)
{
	// We don't add the node if one with the same name already exists
	if (NodeExists(nodeName)) { return false; }

	// You must complete the implementation of this function. Allocate a new 
	// node and add it to your node list(s) and return true on success

	Node *pMem = new Node(nodeName, nodeData);
	if(pMem)
	{
		m_nodes.push_back(pMem);
		return true;
	}
	return false;
}
	
// Adds an edge to the graph that starts at the source node, 
// and goes to the target node.
// If the graph already contains an edge going from the specified 
// source to target node then false is returned and no modification 
// occurs.
// If either or both of the two nodes don't exist in the graph then 
// false is returned and no modification occurs.
// Otherwise the edge will be added. Note that an edge from N1 (source) 
// to N2 (target) is different than an edge from N2 (source) to N1 (target).
bool DirectedGraph::AddEdge(const string& sourceNodeName,
	const string& targetNodeName)
{
	// You must implement this function
	// Make sure to do the following:
	// 1. First make sure both nodes exist. If either one does NOT exist then 
	//    return false and do not add the edge.
	int index1 = FindNode(sourceNodeName), index2 = FindNode(targetNodeName);
	if(index1 == -1 || index2 == -1) // At least one node does not exist
	{
		return false;
	}

	// Check to make sure the edge does not already exist
	for(vector<Edge*>::iterator iter = m_edges.begin(); iter != m_edges.end(); iter++)
	{
		if((*iter)->srcI == index1 && (*iter)->targetI == index2) //Edge already exists
		{
			return false; 
		}
	}

	// 2. Allocate a new edge and add it to the edge list. Remember that the 
	//    the edges refer to nodes by index value, so you'll need to find the 
	//    indices of the source and target nodes based on their names.
	Edge* pMem = new Edge(index1, index2);
	if(!pMem) // Could not allocate memory
	{
		return false;
	}
	m_edges.push_back(pMem);

	// 3. Add the edge as an outgoing edge for the source node and an incoming 
	//    edge for the target node.
	m_nodes[index1]->Out.push_back(pMem);
	m_nodes[index2]->In.push_back(pMem);

	// 4. Return true if you did 2 through 4 successfully.
	return true;
}

// Gets the degree of the node with the specified name. -1 is returned 
// if no such node exists in the graph.
int DirectedGraph::GetDegree(const string& nodeName)
{
	// You must implement this function
	// Remember that the degree of a node is the sum of the number of edges 
	// going to this node plus the number of edges coming out from this node.
	
	//Check if node exists, return -1 if it does not
	int index = FindNode(nodeName);
	if(index == -1) // Node does not exist
	{
		return index;
	}

	// Find degree of node
	int degree = 0;
	for(vector<Edge*>::iterator iter = m_nodes[index]->In.begin(); iter != m_nodes[index]->In.end(); iter++)
	{
		degree++;
	}
	for(vector<Edge*>::iterator iter = m_nodes[index]->Out.begin(); iter != m_nodes[index]->Out.end(); iter++)
	{
		degree++;
	}

	return degree;
}

bool DirectedGraph::GetShortestPath(const string& startNode, const string& endNode, 
	bool nodeDataInsteadOfName, vector<string>& traversalList)
{
	// You must implement this function
	// Find a path from the starting node to the ending node
	// If no such path exists, return false and you're done
	// If such a path DOES exist, you need to add either each node's Name (if 
	// nodeDataInsteadOfName is false) or each node's Data (if nodeDataInsteadOfName 
	// is true) to the traversal list. So after the function completes the traversal 
	// list consists of values (Name or Data) from nodes along the shortest path 
	// from start to end.

	// Check to see if startNode & endNode exists
	int startI = FindNode(startNode);
	int endI = FindNode(endNode);
	if(startI == -1 || endI == -1) // One (or both) of the nodes does not exist
	{
		return false;
	}

	vector<unsigned int> vals;
	vector<bool> visited;

	// Initalize startNode value to 0 and all other node values to infinity
	for(vector<Node*>::iterator iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
	{
		if(iter - m_nodes.begin() == startI)
		{
			vals.push_back(0);
		}
		else
		{
			vals.push_back(-1);
		}
		visited.push_back(false);
	}

	// Set pCurrent to start node
	Node* pCurrent = *(m_nodes.begin() + startI);
	Node* pStartNode = pCurrent;
	int currentIndex = startI;
	Node* pEndNode = *(m_nodes.begin() + endI);
	int lowestUnchecked = startI;

	// While pCurrent != endNode && lowest Unchecked != infinity traverse through graph
	// to enter path values
	while(pCurrent != pEndNode && vals[lowestUnchecked] != -1)
	{
		// Iterate through each outbound neighbor
		for(vector<Edge*>::iterator iter = pCurrent->Out.begin(); iter != pCurrent->Out.end(); iter++)
		{
			// If pCurrent's val + 1 < neighbors val, set neighbors val to pCurrent + 1
			if((vals[currentIndex] + 1) < vals[(*iter)->targetI])
			{
				vals[(*iter)->targetI] = vals[currentIndex] + 1;
			}
		}
		// Set pCurrent to checked
		visited[currentIndex] = true;

		// Find node with lowest value that hasn't been visited
		vector<bool>::iterator checkedIter = visited.begin();
		int count = 0;
		while(*checkedIter)
		{
			count++;
			checkedIter++;
		}
		lowestUnchecked =  count;
		checkedIter++;
		count++;
		while(checkedIter != visited.end())
		{
			if(!(*checkedIter) &&  vals[count] < vals[lowestUnchecked])
			{
				lowestUnchecked = count;
			}
			count++;
			checkedIter++;
		}
		pCurrent = *(m_nodes.begin() + lowestUnchecked);
		currentIndex = lowestUnchecked;
	}
	// If pCurrent does not equal end node then no path exists
	if(pCurrent != pEndNode)
	{
		return false;
	}
	// Create array with list of index arrays of shortest path (backwards)
	int size = vals[lowestUnchecked]+1;
	int current = size-1;
	while(size != 0)
	{
		int *list = new int[size];
		
	}

}

bool DirectedGraph::NodeExists(const string& nodeName)
{
	// You must implement this function
	// Return true if the node exists in the graph, false otherwise.
	if(FindNode(nodeName) == -1)
	{
		return false;
	}
	return true;
}

// Locates node with given name in node array
// Returns array index of node (returns -1 if node does not exist)
int DirectedGraph::FindNode(const string& nodeName)
{
	bool found = false;
	int index = -1;
	vector<Node*>::iterator iter = m_nodes.begin();
	while (!found && iter != m_nodes.end())
	{
		if((*iter)->Name == nodeName)
		{
			found = true;
			index = (iter - m_nodes.begin());
		}
		iter++;
	}
	return index;
}