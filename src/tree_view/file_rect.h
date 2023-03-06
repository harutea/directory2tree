#ifndef FILERECT_H
#define FILERECT_H

#include <QGraphicsItem>
#include <QGraphicsObject>

class FileRect : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

private:
    QString file_path;
    QString fore_dir_path;

public:
    FileRect(QString fore_dir_path, QString file_path);
    ~FileRect();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked(QString file_path);
};

#endif // FILERECT_H
