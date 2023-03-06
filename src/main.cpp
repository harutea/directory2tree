#include <QApplication>
#include <main_window/main_window.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font;
    font.setFamily(QString("Arial"));
    app.setFont(font);

    MainWindow w;
    w.setAttribute(Qt::WA_QuitOnClose);
    w.show();

    return app.exec();
}
