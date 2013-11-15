// Cpt S 223 - Homework 9
// DirectedGraph.h

#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <string>
#include <vector>

using namespace std;

class DirectedGraph
{
private:
	// Represents an edge going from source to target node (source -> target)
	struct Edge
	{
		int srcI; // Index of source node
		int targetI; // Index of target node
		Edge(int sourceNodeIndex, int targetNodeIndex)
		{
			srcI = sourceNodeIndex;
			targetI = targetNodeIndex;
		}
	};

	struct Node
	{
		string Name;
		string Data;
		Node(const string& nodeName, const string& nodeData)
		{
			Name = nodeName;
			Data = nodeData;
		}

		// List of incoming edges to this node
		vector<Edge*> In;

		// List of edges going out from this node
		vector<Edge*> Out;
	};

	// We need a list of nodes and edges
	vector<Node*> m_nodes;
	vector<Edge*> m_edges;

public:
	DirectedGraph();
	virtual ~DirectedGraph();
	bool AddNode(const string& nodeName, const string& nodeData);
	
	// Adds an edge to the graph that starts at the source node 
	// and goes to the target node.
	// If the graph already contains an edge going from the specified 
	// source to target node then false is returned and no modification 
	// occurs.
	// If either or both of the two nodes don't exist in the graph then 
	// false is returned and no modification occurs.
	// Otherwise the edge will be added. Note that an edge from N1 (source) 
	// to N2 (target) is different than an edge from N2 (source) to N1 (target).
	bool AddEdge(const string& sourceNodeName, const string& targetNodeName);

	// Gets the degree of the node with the specified name. -1 is returned 
	// if no such node exists in the graph.
	int GetDegree(const string& nodeName);

	bool GetShortestPath(const string& startNode, const string& endNode, 
		bool nodeDataInsteadOfName, vector<string>& traversalList);

	bool NodeExists(const string& nodeName);

	// Finds node and returns array index (returns -1 if node not found)
	int FindNode(const string& nodeName);
};

#endif