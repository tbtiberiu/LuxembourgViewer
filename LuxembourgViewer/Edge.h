#pragma once
#include "Node.h"

class Edge
{
public:
    Edge();
    Edge(Node* firstNode, Node* secondNode, int length = 0);
    Node* getFirstNode() const;
    Node* getSecondNode() const;
    int getLength() const;
    friend bool operator==(const Edge& edgeA, const Edge& edgeB);

public:
    static const int arrowSize = 3;

private:
    Node* m_firstNode, * m_secondNode;
    int m_length;
};
