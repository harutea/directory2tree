#include "tree_view.h"
#include "ui_tree_view.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFileDialog>
#include <tree_generator/tree_generator.h>
#include "file_rect.h"

TreeView::TreeView(QWidget *parent, QString dir_path) :
    QWidget(parent),
    ui(new Ui::TreeView)
{
    scene = new QGraphicsScene(this);

    ui->setupUi(this);

    ui->graphics_view->setScene(scene);
    ui->graphics_view->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
//    scene->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));

    QStringList dir_string_list = dir_path.split("/");
    this->root_name = *(dir_string_list.end()-1);

    this->setWindowTitle("directory2tree - "+ dir_path);
    this->setAttribute(Qt::WA_DeleteOnClose);

    label_file_path.setGeometry(20, 20, 640, 300);
    label_file_path.setWordWrap(true);
    label_file_path.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label_file_path.setFont(QFont("Arial", 24));
    label_file_path.setParent(ui->graphics_view);
    label_file_path.setStyleSheet("color: #000000");
    label_file_path.setText("-");
    label_file_path.setTextInteractionFlags(Qt::TextSelectableByMouse);
    label_file_path.setAttribute( Qt::WA_TransparentForMouseEvents );
    pen.setColor(Qt::white);
}

TreeView::~TreeView()
{
    delete ui;
    for(auto file_rect : vector_file_rect) delete file_rect;
    for(auto text_item : vector_text_item) delete text_item;
}

void TreeView::closeEvent(QCloseEvent* event)
{
    emit tree_view_close();
}

void TreeView::drawFile(QString file_path, int x, int y, int w, int h, qreal rotation, QString color)
{
    brush = QColor(color);
    pen.setWidth(1);
    FileRect* file_rect = new FileRect(TreeGenerator::getForeDirPath(), file_path);
    vector_file_rect.push_back(file_rect);

    file_rect->setRect(x, y, w, h);
    file_rect->setPen(pen);
    file_rect->setBrush(brush);
    file_rect->setTransformOriginPoint(x+w/2, y+h/2);
    file_rect->setRotation(rotation);
    this->connect(file_rect, SIGNAL(clicked(QString)), SLOT(on_item_clicked(QString)));
    scene->addItem(file_rect);
}

void TreeView::drawText(int x, int y, int size, QString text)
{
    QGraphicsTextItem *text_item = new QGraphicsTextItem;
    vector_text_item.push_back(text_item);
    text_item->setDefaultTextColor("black");
//    text_item->setOpacity(1);
    text_item->setPos(x, y);
    text_item->setPlainText(text);
    text_item->setFont(QFont("Arial", size));
    scene->addItem(text_item);
}

void TreeView::on_drawing_finished()
{
    drawText(570, 600, 17, "directory2tree\nby leafmelt");
}

void TreeView::on_item_clicked(QString file_path)
{
    label_file_path.setText(file_path);
}

void TreeView::setFilePathLabelText(QString text)
{
    label_file_path.setText(text);
}

void TreeView::on_btn_save_image_clicked()
{
    if(!scene) throw std::runtime_error("Runtime Error..");

    QImage image(scene->width()+60, scene->height()+60,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
    painter.end();
    QString filename = QFileDialog::getSaveFileName(this, "Save as Image", QCoreApplication::applicationDirPath() + "/tree-" + root_name +".png", tr("Images (*.png"));
    if(filename != "") image.save(filename);
}

void TreeView::setSceneLarge()
{
    if(scene) scene->setSceneRect(QRectF(scene->sceneRect().x(), scene->sceneRect().y(), scene->sceneRect().width()+100, scene->sceneRect().height()+100));
}
