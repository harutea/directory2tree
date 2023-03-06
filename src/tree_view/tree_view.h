#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <vector>

#include <QLabel>
#include <QWidget>
#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>

#include "file_rect.h"

namespace Ui {
class TreeView;
}

class TreeView : public QWidget
{
    Q_OBJECT

private:
    Ui::TreeView *ui;
    QGraphicsScene *scene;
    QString root_name;
    QLabel label_file_path;
    QBrush brush;
    QPen pen;
    std::vector<FileRect*> vector_file_rect;
    std::vector<QGraphicsTextItem*> vector_text_item;

public:
    explicit TreeView(QWidget *parent = nullptr, QString root_name="");
    ~TreeView();
    void closeEvent(QCloseEvent *event);

    void drawFile(QString file_path, int, int, int, int, qreal, QString);
    void drawText(int, int, int, QString);
    void setSceneLarge(); // doesn't do the expected work
    void setFilePathLabelText(QString text);

signals:
    void tree_view_close();

public slots:
    void on_drawing_finished();
    void on_item_clicked(QString file_path);

private slots:
    void on_btn_save_image_clicked();
};

#endif // TREEVIEW_H
