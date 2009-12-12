#ifndef STAGEOBJECT_H
#define STAGEOBJECT_H

#include <QtGui>
struct StageObject : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
public:
    StageObject() {}

    QPixmap pixmap() const { return d.pixmap; }
    void setPixmap(const QPixmap &pixmap) { d.pixmap = pixmap; update(); }

    QString text() const { return d.text; }
    void setText(const QString &text) { d.text = text; update(); }

    QPen pen() const { return d.pen; }
    void setPen(const QPen &pen) { d.pen = pen; update(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    struct Data {
        QString text;
        QPixmap pixmap;
        QPen pen;
    } d;
};

struct ActorObject : public StageObject
{
    Q_OBJECT
//    Q_PROPERTY(QString name READ name WRITE setName)
public:
    ActorObject() : StageObject() {}

};

#endif
