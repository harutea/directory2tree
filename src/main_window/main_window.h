#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <tree_generator/console_dialog.h>
#include <tree_generator/tree_generator.h>
#include <tree_view/tree_view.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QPixmap image_logo;
    ConsoleDialog* console_dialog;
    QTextEdit* console_text_edit;
    TreeView* tree_view;
    TreeGenerator tree_generator;
    QThread thread_tree_generator;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_select_clicked();
    void on_btn_generate_clicked();
    void finish_generate(bool abort);
    void console_write(QString);
    void stop_tree_generate();
    void draw_file(QString file_path, int x, int y, int w, int h, qreal rotation);
    void draw_dir(QString dir_path, int x, int y, int w, int h, qreal rotation);
    void draw_text(int x, int y, int size, QString text);
};
#endif // MAINWINDOW_H
