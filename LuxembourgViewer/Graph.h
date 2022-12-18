#pragma once
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <QTextStream>
#include "Edge.h"

class Graph
{
public:
        struct OrderByLength
        {
			bool operator() (std::pair<int, int> const& a, std::pair<int, int> const& b) { return a.second > b.second; }
        };

public:
    Graph();
    void addNode(Node* node);
    void addEdge(const Edge& edge);
    int getNodesCount() const;
    Node* getNodeAt(QPointF pos);
    Node* getNode(int info);
    std::vector<Node*> getNodes() const;
    std::vector<Edge> getEdges() const;
    bool hasEdge(Edge edge) const;
    std::vector<Node*> shortestPath(int from, int to);

private:
    int m_nodesCount = 0;
    std::vector<Node*> m_nodes;
    std::vector<Edge> m_edges;
};
