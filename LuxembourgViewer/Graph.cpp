#include "Graph.h"
#include <QDebug>

Graph::Graph()
{
    // empty
}

void Graph::addNode(Node* node)
{
    m_nodes.push_back(node);
    m_nodesCount++;
}

void Graph::addEdge(const Edge& edge)
{
    if (hasEdge(edge))
        return;

    m_edges.push_back(edge);
}

int Graph::getNodesCount() const
{
    return m_nodesCount;
}

Node* Graph::getNodeAt(QPointF pos)
{
    Node n(pos);

    for (Node* nn : m_nodes)
        if (Node::getDistance(n, *nn) <= Node::radius)
            return nn;

    return nullptr;
}

Node* Graph::getNode(int info) {
    for (Node* node : m_nodes) {
        if (node->getInfo() == info) {
            return node;
        }
    }

    return nullptr;
}

std::vector<Node*> Graph::getNodes() const
{
    return m_nodes;
}

std::vector<Edge> Graph::getEdges() const
{
    return m_edges;
}

bool Graph::hasEdge(Edge edge) const
{
    for (const Edge& x : m_edges)
        if (edge == x)
            return true;
    return false;
}

std::vector<Node*> Graph::shortestPath(int from, int to)
{
    std::vector<Node*> path;
    std::vector<int> dist(m_nodesCount, INT_MAX);
    dist[from] = 0;
    std::vector<int> prev(m_nodesCount, -1);
    std::unordered_set<int> visited;
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, OrderByLength> q;
    q.push({ from, 0 });

    while (!q.empty()) {
        do {
            if (q.empty())
                return path;
            from = q.top().first;
            q.pop();
        } while (visited.find(from) != visited.end());
        visited.insert(from);

        if (from == to) {
            while (from != -1) {
                path.push_back(getNode(from));
                from = prev[from];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const Edge& edge : m_edges) {
            if (edge.getFirstNode()->getInfo() == from) {
                int to = edge.getSecondNode()->getInfo();
                if (dist[to] > dist[from] + edge.getLength()) {
                    dist[to] = dist[from] + edge.getLength();
                    prev[to] = from;
                    q.push({ to, dist[to] });
                }
            }
        }
    }

    return path;
}
