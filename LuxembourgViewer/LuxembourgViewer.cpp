#include "LuxembourgViewer.h"

LuxembourgViewer::LuxembourgViewer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    showMaximized();
    ui.loadingLabel->setText("Reading Luxembourg Map...");
    readXMLFile();

    ui.loadingLabel->setText("Painting Luxembourg Map...");
    luxembourgMap = QPixmap(QWidget::width(), QWidget::height());
    luxembourgMap.fill(Qt::white);

    QPainter painter(&luxembourgMap);
    paintEdges(painter);
    update();
}

LuxembourgViewer::~LuxembourgViewer()
{}

void LuxembourgViewer::readXMLFile() {
    QFile file("../LuxembourgViewer/LuxembourgMap.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file for reading";
        return;
    }

    QXmlStreamReader xmlReader(&file);
    long minLong = 10000000;
    long maxLong = -10000000;
    long minLat = 10000000;
    long maxLat = -10000000;
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == QLatin1String("node"))
            {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                int id = attributes.value("id").toInt();
                int latitude = attributes.value("latitude").toInt();
                int longitude = attributes.value("longitude").toInt();

                if (latitude < minLat)
                {
                    minLat = latitude;
                }
                else if (latitude > maxLat)
                {
                    maxLat = latitude;
                }

                if (longitude < minLong)
                {
                    minLong = longitude;
                }
                else if (longitude > maxLong)
                {
                    maxLong = longitude;
                }

                QPointF coord(longitude, latitude);
                graph.addNode(new Node(coord, id));
            }

            if (xmlReader.name() == QLatin1String("arc"))
            {
                QXmlStreamAttributes attributes = xmlReader.attributes();
                int from = attributes.value("from").toInt();
                int to = attributes.value("to").toInt();
                int length = attributes.value("length").toInt();

                Node* fromNode = graph.getNode(from);
                Node* toNode = graph.getNode(to);

                if (fromNode == nullptr || toNode == nullptr) {
                    continue;
                }

                Edge newEdge(fromNode, toNode, length);
                graph.addEdge(newEdge);
            }
        }
    }
    
    for (Node* n : graph.getNodes()) {
        QPointF coord = n->getCoordinate();
        coord.setX((coord.x() - minLong) * (QWidget::width() - 20) / (maxLong - minLong) + 10);
        coord.setY((coord.y() - minLat) * (QWidget::height() - 20) / (maxLat - minLat) + 10);
        n->setCoordinate(coord);
    }

    qDebug() << "Finished reading!";

    if (xmlReader.hasError()) {
        qDebug() << "Error: Failed to parse file" << xmlReader.errorString();
    }

    file.close();
}

void LuxembourgViewer::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton) {
        firstSelectedNode = graph.getNodeAt(ev->pos());
    }
    else if (ev->button() == Qt::RightButton) {
        secondSelectedNode = graph.getNodeAt(ev->pos());
    }

    update();
}

void LuxembourgViewer::paintNodes(QPainter& p)
{
    std::vector<Node*> nodes = graph.getNodes();
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    p.setPen(pen);

    for (const Node* n : nodes) {
        QPointF coord = n->getCoordinate();

        QRect r(coord.x() - Node::radius,
            coord.y() - Node::radius,
            2 * Node::radius,
            2 * Node::radius);
        p.drawEllipse(r);
    }
}

void LuxembourgViewer::paintEdges(QPainter& p)
{
    std::vector<Edge> edges = graph.getEdges();

    for (const Edge& e : edges) {
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(1);
        p.setPen(pen);

        QLineF line(e.getFirstNode()->getCoordinate(), e.getSecondNode()->getCoordinate());
        line.setLength(line.length());
        line.setP1(line.p1() + e.getSecondNode()->getCoordinate() - line.p2());
        p.drawLine(line);
    }
}

void LuxembourgViewer::paintSelectedNodes(QPainter& p)
{
    if (firstSelectedNode != nullptr) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(2);
        p.setPen(pen);

        QPointF coord = firstSelectedNode->getCoordinate();
        QRect r(coord.x() - Node::radius,
            coord.y() - Node::radius,
            2 * Node::radius,
            2 * Node::radius);
        p.drawEllipse(r);
    }

    if (secondSelectedNode != nullptr) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(2);
        p.setPen(pen);

        QPointF coord = secondSelectedNode->getCoordinate();
        QRect r(coord.x() - Node::radius,
            coord.y() - Node::radius,
            2 * Node::radius,
            2 * Node::radius);
        p.drawEllipse(r);
    }
}

void LuxembourgViewer::paintPath(QPainter& p)
{
    ui.loadingLabel->setText("Painting Shortest Path...");

    if (path.empty()) {
        return;
    }

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    p.setPen(pen);

    for (size_t i = 0; i < path.size() - 1; i++) {
        Node* n1 = path[i];
        Node* n2 = path[i + 1];

        QLineF line(n1->getCoordinate(), n2->getCoordinate());
        line.setLength(line.length());
        line.setP1(line.p1() + n2->getCoordinate() - line.p2());
        p.drawLine(line);
    }
}

void LuxembourgViewer::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawPixmap(0, 0, luxembourgMap);
    paintSelectedNodes(p);
    paintPath(p);
    ui.loadingLabel->setText("");
}

void LuxembourgViewer::on_pathButton_clicked()
{
    if (firstSelectedNode == nullptr || secondSelectedNode == nullptr) {
        qDebug() << "Error: Not enough nodes selected!";
        return;
    }

    ui.loadingLabel->setText("Searching Shortest Path...");
    path = graph.shortestPath(firstSelectedNode->getInfo(), secondSelectedNode->getInfo());

    if (path.empty()) {
        qDebug() << "Error: No path found!";
        return;
    }

    qDebug() << "Path found!";
    update();
}
