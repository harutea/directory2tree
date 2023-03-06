#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H

#include <random>

#include <QString>
#include <QThread>
#include <QTextEdit>

class TreeGenerator : public QThread
{
    Q_OBJECT

private:
    const int kRootX = 500, kRootY = 500, kRootW=33, kRootH=230;
    const int kDiffW=6, kDiffH=25;
    const int kLeafW = 27, kLeafH = 40;
    const int kMinBranchW = 12, kMinBranchH = 40;
    const int kRandomRotationLeft = -30, kRandomRotationRight = 30;
    const int kRotationRange = 150;
    static QString directory_path, fore_dir_path, root_name;

    std::function<int(void)> rand_generator;
    bool abort=false;

public:
    TreeGenerator();
    ~TreeGenerator();

    void setPath(QString path);
    void run();
    void searchDirAndDraw(int depth, QString dir_name, int x, int y, int w, int h, qreal rotation);
    void setAbort(bool abort);
    static QString getDirectoryPath();
    static QString getForeDirPath();
    static QString getRootDir();

signals:
    void finish(bool abort);
    void console_write(QString);
    void draw_file(QString file_path, int x, int y, int w, int h, qreal rotation);
    void draw_dir(QString dir_path, int x, int y, int w, int h, qreal rotation);
    void draw_text(int x, int y, int size, QString text);
};

#endif // TREEGENERATOR_H
