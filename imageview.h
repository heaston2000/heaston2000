#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <set>

class QSlider;
class QCheckBox;

class ImageView : public QGraphicsView
{

    QGraphicsScene scene;
    QImage image; // image loaded by the user and displayed to the user
    QGraphicsPixmapItem *pMap;
    QGraphicsPixmapItem *zoomMap;
    QPointF mouse;
    std::set<QPoint> objPoints;
    QSlider *toleranceSlider;
    QCheckBox *rollingTolerance;
    QCheckBox *innerEdges;
    QCheckBox *fill;
    QPolygonF border;
    QPainter paint;
    int width, height;
    QPixmap editMap; // saveable version of the image displayed to the user
    QPainter *editPaint; // tool used to edit this saveable version
    std::vector<QPoint> edge;



public:
    ImageView(QSlider *_tolerance, QCheckBox *_rollTolerance, QCheckBox *_innerEdgeDetect, QCheckBox *_fill);
    ~ImageView();
    QString fName;

    virtual void mousePressEvent(QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseMoveEvent(QMouseEvent *evt) override;

    void mouseHoverEvent(QMouseEvent *evt);
    void setImage(QImage image);
    void detectObj(QPoint start);
    void saveEdit();
    void edgeWalk(); // outer edge walk
    void edgeWalk(std::set<QPoint> &); // inner edge walk
    void setFName(QString);
    void allShapes();


    friend bool operator<(const QPoint &p1, const QPoint &p2);
};

#endif // IMAGEVIEW_H
