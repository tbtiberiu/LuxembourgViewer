#include "Edge.h"

Edge::Edge()
	: m_firstNode(0)
	, m_secondNode(0)
    , m_length(0)
{
    // empty
}

Edge::Edge(Node* firstNode, Node* secondNode, int length)
{
    m_firstNode = firstNode;
    m_secondNode = secondNode;
    m_length = length;
}

Node* Edge::getFirstNode() const
{
    return m_firstNode;
}

Node* Edge::getSecondNode() const
{
    return m_secondNode;
}

int Edge::getLength() const
{
    return m_length;
}

bool operator==(const Edge& edgeA, const Edge& edgeB)
{
    return edgeA.m_firstNode == edgeB.m_firstNode && edgeA.m_secondNode == edgeB.m_secondNode;
}
