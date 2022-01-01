#include "imageview.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QtWidgets>

using namespace std;
ImageView::ImageView(QSlider *_toleranceSlider, QCheckBox *_rollingTolerance, QCheckBox *_innerEdges, QCheckBox *_fill) {
    pMap = nullptr;
    setRenderHint(QPainter::Antialiasing);
    setScene(&scene);
    zoomMap = scene.addPixmap(QPixmap());
    zoomMap->setScale(10);
    zoomMap->setZValue(100);
    setMouseTracking(true);
    toleranceSlider = _toleranceSlider;
    rollingTolerance = _rollingTolerance;
    innerEdges = _innerEdges;
    fill = _fill;
}

ImageView::~ImageView() {
}

void ImageView::mouseHoverEvent(QMouseEvent *) {
}

void ImageView::mousePressEvent(QMouseEvent *) {
    mouse = mapToScene(mapFromGlobal(QCursor::pos())); //takes global cursor & maps it to the scene coords
    QRgb color = image.pixel(mouse.toPoint());
    detectObj(mouse.toPoint());
    edgeWalk(); // outer edge walk
    QVector<QPointF> edgeF;
    for (QPoint p : edge) edgeF.push_back(p);
    border = QPolygonF(edgeF);
    QPen pen = QPen(0xFFFFFF ^ color); // invert the color so the border is visible
    pen.setWidth(2);
    QBrush brush;
    if (fill->isChecked()) brush = QBrush(color);
    if (!fill->isChecked()) brush = Qt::NoBrush;
    scene.addPolygon(border, pen, brush);

    // For editing the saveable version of the image:
    editPaint->setPen(pen);
    editPaint->setBrush(brush);
    editPaint->drawPolygon(border);

    if (innerEdges->isChecked()) { // Inner Edge Detection:
        QVector<QPointF> inEdgeF;
        vector<QPoint> inEdge;
        set<QPoint> inBorderPts;
        QPolygonF inBorder;
        for (QPoint pt1 : objPoints) {
            if (objPoints.find(QPoint(pt1.x(), pt1.y()+1)) == objPoints.end() ||
                    objPoints.find(QPoint(pt1.x(), pt1.y()-1)) == objPoints.end() ||
                    objPoints.find(QPoint(pt1.x()+1, pt1.y())) == objPoints.end() ||
                    objPoints.find(QPoint(pt1.x()-1, pt1.y())) == objPoints.end()) {
                if (edgeF.count(QPointF(pt1)) == 0) inBorderPts.insert(pt1);
            }
        }
        set<QPoint> innerArchive;
        int avgY;
        int avgX;
        int ptCounter;
        while (inBorderPts.size() != 0) {
            if (innerArchive.find(*inBorderPts.begin()) == innerArchive.end()) {
                edgeWalk(inBorderPts);
                ptCounter=0;
                avgY=0;
                avgX=0;
                for (QPoint p : inEdge) {
                    ++ptCounter;
                    inEdgeF.push_back(p);
                    innerArchive.insert(p);
                    avgY += p.y();
                    avgX += p.x();
                }
                if (ptCounter) {
                    avgY = avgY/ptCounter;
                    avgX = avgX/ptCounter;
                } color = image.pixel(QPoint(avgX, avgY));
                if (fill->isChecked()) {
                    brush = QBrush(color);
                    editPaint->setBrush(brush);
                }

                scene.addPolygon(QPolygonF(inEdgeF), pen, brush);
                editPaint->drawPolygon(QPolygonF(inEdgeF));

                inEdgeF.clear();
            } inBorderPts.erase(inBorderPts.begin());
        }
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *) {
}

void ImageView::mouseMoveEvent(QMouseEvent *) {
    if (!pMap) return;
    mouse = mapToScene(mapFromGlobal(QCursor::pos()));
    if (mouse.x() <= 0 || mouse.y() <= 0 || mouse.x() >= image.width() || mouse.y() >= image.height()) return;
    QPoint mousePt = mouse.toPoint();
    QRect zoomedPixels = QRect(mousePt-QPoint(1, 1), mousePt+(QPoint(1, 1)));
    zoomMap->setPixmap(pMap->pixmap().copy(zoomedPixels));
    zoomMap->setPos(mouse + QPoint(5, -10));
}

void ImageView::setImage(QImage _image) {
    image = _image;
    delete pMap;
    pMap = scene.addPixmap(QPixmap::fromImage(_image));
    width = image.width();
    height = image.height();
    qDebug() << image.size();
    if (!fName.isEmpty()) editMap = QPixmap(fName);
}

void ImageView::detectObj(QPoint start){
    set<QPoint> toDo;
    QPoint mPos = start;
    QRgb rgb = image.pixel(start);
    int yMax = -1;
    int xMax = -1;
    int xMin = image.width();
    int yMin = image.height();
    int r0; //r0, g0 and b0 are the red, green and blue values that we will use to decide if pixels are accepted
    int g0;
    int b0;

    if (!rollingTolerance->isChecked()) {
        r0 = (rgb&0xff0000) >> 16;
        g0 = (rgb&0xff00) >> 8;
        b0 = (rgb&0xff);
    }
    int tolerance = toleranceSlider->value();

    objPoints.clear();
    toDo.insert(mPos);
    objPoints.insert(mPos);

    int rTemp, gTemp, bTemp;
    int rTotal = 0;
    int gTotal = 0;
    int bTotal = 0;
    int count = 0;
    while (!toDo.empty()) {
        QPoint p0 = *toDo.begin();
        QRgb rgb0 = image.pixel(p0);
        if (rollingTolerance->isChecked()) {
            ++count;
            rTemp = (rgb0&0xff0000) >> 16;
            gTemp = (rgb0&0xff00) >> 8;
            bTemp = (rgb0&0xff);
            rTotal += rTemp;
            gTotal += gTemp;
            bTotal += bTemp;
            r0 = rTotal/count;
            g0 = gTotal/count;
            b0 = bTotal/count;
        }
        toDo.erase(toDo.begin());
        if (p0.y() > yMax)
            yMax = p0.y();
        if (p0.x() > xMax)
            xMax = p0.x();
        if (p0.x() < xMin)
            xMin = p0.x();
        if (p0.y() < yMin)
            yMin = p0.y();
        //Check all adjacent pixels:
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx || dy) {
                    QPoint p = p0 + QPoint(dx,dy);
                    if (p.x() <= 0 || p.y() <= 0 || p.x() >= image.width() || p.y() >= image.height()) {
                        break;
                    }

                    if (objPoints.find(p) == objPoints.end()) {
                        QRgb pxlClkd = image.pixel(p);
                        int r = (pxlClkd&0xff0000) >> 16;
                        int g = (pxlClkd&0xff00) >> 8;
                        int b = (pxlClkd&0xff);
                        if ((abs(r0 - r) + abs(g0 - g) + abs(b0 - b)) <= tolerance) {
                            toDo.insert(p);
                            objPoints.insert(p);
                        }
                    }
                }
            }
        }
    }
}

bool operator<(const QPoint &p1, const QPoint &p2) { //THIS IS THE CORRECT DEFINITION FOR THE '<' OPERATOR
    int deltaX, deltaY;
    if((deltaY = p1.y() - p2.y()))
        return (deltaY < 0);
    if((deltaX = p1.x() - p2.x()))
        return (deltaX < 0);
    return false;
}

void ImageView::edgeWalk() //Outer Edge walk
{
        edge.clear();
        QPoint start = *objPoints.rbegin();
        QPoint position = start;
        int dx = 0;
        int dy = -1;
        do { //
            if (objPoints.find(QPoint(position.x()+dx-dy, position.y()+dy+dx)) != objPoints.end()) {
                edge.push_back(position);
                position = QPoint(position.x()+dx-dy, position.y()+dy+dx);
                //turn right
                swap(dx, dy);
                dx *= -1;
            } else if (objPoints.find(QPoint(position.x()+dx, position.y()+dy)) != objPoints.end()) {
                position = QPoint(position.x()+dx, position.y()+dy);
                edge.push_back(position);
            } else {
                // turn left
                swap(dy, dx);
                dy*=-1;
            }
        } while (start.x() != position.x() || start.y() != position.y() || dy != -1 || dx != 0);
}
void ImageView::edgeWalk(set<QPoint> &borderPts) { // Innner Edge walk
        edge.clear();
        QPoint position = *borderPts.begin();
        QPoint start = position;

        int dx = 0;
        int dy = 1;
        do {
            if (borderPts.find(QPoint(position.x()+dx-dy, position.y()+dy+dx)) != borderPts.end()) {
                edge.push_back(position);
                position = QPoint(position.x()+dx-dy, position.y()+dy+dx);
                //turn right
                swap(dx, dy);
                dx *= -1;
            } else if (borderPts.find(QPoint(position.x()+dx, position.y()+dy)) != borderPts.end()) {
                position = QPoint(position.x()+dx, position.y()+dy);
                edge.push_back(position);
            } else {
                // turn left
                swap(dy, dx);
                dy*=-1;
            }
        } while (start.x() != position.x() || start.y() != position.y() || dy != 1 || dx != 0);

}

void ImageView::saveEdit() {
    QString editName = fName.chopped(4) + "_edit.png";
    QFile editFile(editName);
    editFile.open(QIODevice::WriteOnly);
    editMap.save(&editFile, "PNG");
}
void ImageView::setFName(QString _fName){
    fName.clear();
    fName = _fName;
    if (!fName.isEmpty()) editMap = QPixmap(fName);
    QPainter *_editPaint = new QPainter(&editMap);
    editPaint = _editPaint; // editPaint is the tool used to alter the saveable version of the image
}
void ImageView::allShapes() {
    set<QPoint> ptArchive;
    for (int i=1; i<(height); ++i) {
        for (int j=1; j<(width); ++j) {
            if (ptArchive.count(QPoint(i,j)) == 0) {
                detectObj(QPoint(i, j));
                edgeWalk();
                QVector<QPointF> edgeF;
                for (QPoint p : edge) edgeF.push_back(p);
                border = QPolygonF(edgeF);
                QPen pen = QPen();
                pen.setWidth(2);
                QBrush brush;
                if (fill->isChecked()) brush = QBrush(image.pixel(i, j));
                if (!fill->isChecked()) brush = Qt::NoBrush;
                scene.addPolygon(border, pen, brush);
                for (QPoint p : objPoints) ptArchive.insert(p);
            }
        }
    }
}

