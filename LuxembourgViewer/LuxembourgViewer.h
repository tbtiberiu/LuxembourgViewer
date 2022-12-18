#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LuxembourgViewer.h"
#include "Graph.h"
#include <math.h>

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QDrag>
#include <QFile>
#include <QInputDialog>
#include <QPainterPath>
#include <QXmlStreamReader>

class LuxembourgViewer : public QMainWindow
{
    Q_OBJECT

public:
    LuxembourgViewer(QWidget *parent = nullptr);
    ~LuxembourgViewer();

    void readXMLFile();
    void mousePressEvent(QMouseEvent* ev);
    void paintEvent(QPaintEvent* p);
    void paintNodes(QPainter& p);
    void paintEdges(QPainter& p);
    void paintSelectedNodes(QPainter& p);
    void paintPath(QPainter& p);
    
private slots:
	void on_pathButton_clicked();

private:
    Graph graph;
    Ui::LuxembourgViewerClass ui;
    QPixmap luxembourgMap;

    Qt::MouseButton pressedButton;
    Node* firstSelectedNode = nullptr;
    Node* secondSelectedNode = nullptr;
    std::vector<Node*> path;
};