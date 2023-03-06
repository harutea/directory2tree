#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QTextEdit>

class ConsoleDialog : public QDialog
{
    Q_OBJECT

private:
    QTextEdit *console_text_edit;

public:
    explicit ConsoleDialog(QDialog *parent = nullptr, QString dir_path_text="");
    void print(QString text);
    void closeEvent(QCloseEvent *) override;

signals:
    void console_closed();

public slots:
    void on_tree_view_closed();
};

#endif // CONSOLEDIALOG_H
