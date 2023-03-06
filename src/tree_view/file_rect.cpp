#include "file_rect.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

FileRect::FileRect(QString fore_dir_path, QString file_path)
{
    this->fore_dir_path = fore_dir_path;
    this->file_path = file_path;

    this->setOpacity(0.8);
    this->setFlags(QGraphicsItem::ItemIsMovable);
}

FileRect::~FileRect()
{

}

void FileRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(file_path);
}

void FileRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"double clicked";
    QString file_path_all =  this->fore_dir_path + "/" + this->file_path ;
    QDesktopServices::openUrl( QUrl::fromLocalFile(file_path_all) );
    qDebug()<<file_path_all;
}
