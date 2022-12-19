#pragma once
#include <QPoint>
#include <QColor>
#include <QRandomGenerator>

class Node
{
public:
    Node(int info = -1);
    Node(QPointF coord, int info = -1);
    QPointF getCoordinate() const;
    int getInfo() const;
    void setInfo(int info);
    void setCoordinate(QPointF coord);
    static float getDistance(const Node& nodeA, const Node& nodeB);
    friend bool operator==(const Node& nodeA, const Node& nodeB);

public:
    static const int radius = 5;

private:
    int m_info;
    QPointF m_coord;
};

