#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <tree_generator/console_dialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    tree_generator.moveToThread(&thread_tree_generator);
    connect(&thread_tree_generator, &QThread::finished, &tree_generator, &QObject::deleteLater);

    connect(&tree_generator, SIGNAL(finish(bool)), SLOT(finish_generate(bool)));
    connect(&tree_generator, SIGNAL(console_write(QString)), SLOT(console_write(QString)));
    connect(&tree_generator, SIGNAL(draw_file(QString, int, int, int, int, qreal)), SLOT(draw_file(QString, int, int, int, int, qreal)));
    connect(&tree_generator, SIGNAL(draw_dir(QString, int, int, int, int, qreal)), SLOT(draw_dir(QString, int, int, int, int, qreal)));
    connect(&tree_generator, SIGNAL(draw_text(int, int, int, QString)), SLOT(draw_text(int, int, int, QString)));

    ui->setupUi(this);
    ui->line_directory_path->setText(QDir::homePath());
    image_logo.load(":/images/resource/images/logo1.png");
    ui->label_logo->setPixmap(image_logo.scaled(ui->label_logo->width(), ui->label_logo->height(), Qt::KeepAspectRatio));
    this->statusBar()->setSizeGripEnabled(false);
    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    thread_tree_generator.exit();
    delete ui;
}


void MainWindow::on_btn_select_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Directory", ui->line_directory_path->text(), QFileDialog::ShowDirsOnly);
    if(path!="") ui->line_directory_path->setText(path);
}

void MainWindow::on_btn_generate_clicked()
{
    if(ui->line_directory_path->text() == "") {
        QMessageBox msg_box;
        msg_box.setText("Select a directory.");
        msg_box.exec();
        return;
    }

    /* Make a console dialog */
    console_dialog = new ConsoleDialog(nullptr, ui->line_directory_path->text());
    console_dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(console_dialog, SIGNAL(console_closed()), SLOT(stop_tree_generate()));
    console_dialog->show();

    /* Make a TreeView dialog */
    tree_view = new TreeView(nullptr, ui->line_directory_path->text());
    tree_view->setAttribute(Qt::WA_DeleteOnClose);
    connect(tree_view, SIGNAL(tree_view_close()), console_dialog, SLOT(on_tree_view_closed()));

    /* Set and run TreeGenerator */
    ui->btn_generate->setDisabled(true);
    tree_generator.setAbort(false);
    tree_generator.setPath(ui->line_directory_path->text());
    qDebug()<<TreeGenerator::getForeDirPath();
    tree_generator.start();
}

void MainWindow::finish_generate(bool abort)
{
    if(abort) {
        delete tree_view;
        delete console_dialog;
        tree_view = nullptr;
        console_dialog = nullptr;
        ui->btn_generate->setDisabled(false);
        return;
    }

    if(!tree_view || !console_dialog) throw std::runtime_error("Runtime Error..");

    tree_view->setFilePathLabelText(TreeGenerator::getRootDir());
    tree_view->on_drawing_finished();
    tree_view->show();
    console_dialog->setAttribute(Qt::WA_DeleteOnClose);
    ui->btn_generate->setDisabled(false);
}

void MainWindow::console_write(QString text)
{
    if(console_dialog) console_dialog->print(text);
}

void MainWindow::stop_tree_generate()
{
    tree_generator.setAbort(true);
    thread_tree_generator.exit();
    thread_tree_generator.wait();
}

void MainWindow::draw_file(QString file_path, int x, int y, int w, int h, qreal rotation)
{
    if(tree_view) tree_view->drawFile(file_path, x, y, w, h, rotation, "#4CA973");
}

void MainWindow::draw_dir(QString dir_path, int x, int y, int w, int h, qreal rotation)
{
    if(tree_view) tree_view->drawFile(dir_path, x, y, w, h, rotation, "#482218");
}

void MainWindow::draw_text(int x, int y, int size, QString text)
{
    if(tree_view) tree_view->drawText(x, y, size, text);
}
