#include "console_dialog.h"
#include <QLayout>
#include <QTextEdit>

ConsoleDialog::ConsoleDialog(QDialog *parent, QString dir_path_text) : QDialog(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    console_text_edit = new QTextEdit(this);
    console_text_edit->setReadOnly(true);
    layout->setMargin(0);
    layout->addWidget(console_text_edit);
    this->setLayout(layout);
    this->setWindowTitle("directory2tree Console - " + dir_path_text);
    this->setGeometry(100, 100, 450, 350);
}

void ConsoleDialog::print(QString text)
{
    if(console_text_edit) console_text_edit->append(text);
}

void ConsoleDialog::closeEvent(QCloseEvent* event)
{
    emit console_closed();
    event->accept();
}

void ConsoleDialog::on_tree_view_closed()
{
    this->close();
}
