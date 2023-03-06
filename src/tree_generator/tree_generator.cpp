#include "tree_generator.h"

#include <cmath>
#include <ctime>

#include <QDir>
#include <QDebug>
#include <QDialog>
#include <QTextEdit>
#include <QLayout>
#include <QDesktopServices>

#define PI 3.14159265


QString TreeGenerator::directory_path="";
QString TreeGenerator::fore_dir_path="";
QString TreeGenerator::root_name="";

TreeGenerator::TreeGenerator()
{
    std::mt19937 rand_engine((unsigned int)time(NULL));
    std::uniform_int_distribution<int> dist(this->kRandomRotationLeft, this->kRandomRotationRight);
    this->rand_generator = std::bind(dist, rand_engine);
}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::setPath(QString path)
{
    if(*(path.end()-1) == '/') path.remove(path.length()-1,1);
    directory_path = path;
    QDir directory(directory_path);
    QStringList dir_string_list = directory_path.split("/");

    fore_dir_path="";
    for(auto i = dir_string_list.begin(); i != dir_string_list.end()-1; i++) {
        fore_dir_path += "/" + *i;
    }

    root_name = *(dir_string_list.end()-1);
}

void TreeGenerator::run()
{
    qDebug()<<"\nGenerating a Tree...";


    qDebug()<<"ROOT: " + root_name;
    emit console_write("ROOT: " + root_name + ", x: " + QString::number(500) + ", y: " + QString::number(700));

    root_name = root_name == "" ? "~root~" : root_name;
    emit draw_dir(root_name, kRootX, kRootY, kRootW, kRootH, 0);
    emit draw_text(kRootX - root_name.size()/2*20, kRootY+kRootH+10, 30, root_name);

    this->searchDirAndDraw(0, "", kRootX, kRootY+20, kRootW-5, kRootH-60, 0); // search from root

    emit finish(this->abort);
}

void TreeGenerator::searchDirAndDraw(int depth, QString dir_name, int p_x, int p_y, int p_w, int p_h, qreal p_rotation) {
    if(this->abort) return;

    QDir directory(directory_path + dir_name);

    QStringList child_dir_name_list = directory.entryList(QDir::Dirs);
    QStringList child_file_name_list = directory.entryList(QDir::Files);

    if(child_dir_name_list.size()>2) {

        /* Decide differences of rotation */
        int diff_rotation = this->kRotationRange / (child_dir_name_list.size()-2);

        /* Decide directory branch size */
        int c_w = p_w - this->kDiffW > this->kMinBranchW ? p_w - this->kDiffW : this->kMinBranchW;
        int c_h = p_h - this->kDiffH > this->kMinBranchH ? p_h - this->kDiffH : this->kMinBranchH;

        /* Decide order of directory branch and file leaf (left/right) */
        /* and decide initial rotation of children */

        qreal c_rotation = p_rotation;
        int c_rotation_update = rand_generator();
        c_rotation_update -= 20 * (child_dir_name_list.size()-3);
        if(c_rotation_update < -(this->kRotationRange/2)) c_rotation_update = -(this->kRotationRange/2) + rand_generator();
        c_rotation += c_rotation_update;
        if(c_rotation < -90) c_rotation = -90 + rand_generator();
        if(c_rotation + (child_dir_name_list.size()-3)*diff_rotation > 90) c_rotation = 90 - (child_dir_name_list.size()-3)*diff_rotation + rand_generator();

        for(int i=0; i<child_dir_name_list.size(); i++) {

            /* Ignore '.' and '..' */
            if(child_dir_name_list[i]=="." || child_dir_name_list[i]=="..") continue;

            int c_x = p_x + p_w/2 + p_h/2 * sin(PI/180.0 * p_rotation) + c_h/2 * sin(PI/180.0 * c_rotation);
            int c_y = p_y - p_h/2 * cos(PI/180.0 * p_rotation) - c_h/2 * cos(PI/180.0 * c_rotation)+10;

            QString console_text="";
            for(int i=0; i<depth; i++) console_text+="    ";

            console_text += "└ DIR: " + child_dir_name_list[i] + ", x: " + QString::number(c_x) + ", y: " + QString::number(c_y);

            /* Output to debug console */
            qDebug()<<console_text;

            /* Output to user console */
            emit console_write(console_text);

            /* Draw a directory branch */
            emit draw_dir(root_name+dir_name+"/"+child_dir_name_list[i], c_x, c_y, c_w, c_h, c_rotation);

            /* search a child directory */
            searchDirAndDraw(depth+1, dir_name+"/" + child_dir_name_list[i], c_x, c_y, c_w, c_h, c_rotation);

            c_rotation += diff_rotation;
        }

    }

    if(child_file_name_list.size()) {

        int diff_rotation = this->kRotationRange / (child_file_name_list.size());

        qreal c_rotation = p_rotation;
        int c_rotation_update = rand_generator();
        c_rotation_update -= 20 * (child_file_name_list.size()-1);
        if(c_rotation_update < -(this->kRotationRange/2)) c_rotation_update = -(this->kRotationRange/2) + rand_generator();
        c_rotation += c_rotation_update;
        if(c_rotation < -90) c_rotation = -90 + rand_generator();
        if(c_rotation + (child_file_name_list.size()-1)*diff_rotation > 90) c_rotation = 90 - (child_file_name_list.size()-1)*diff_rotation + rand_generator();

        for(int i=0; i<child_file_name_list.size(); i++) {

            int c_x = p_x + p_w/2 + p_h/2 * sin(PI/180.0 * p_rotation) + this->kLeafH/2 * sin(PI/180.0 * c_rotation);
            int c_y = p_y - p_h/2 * cos(PI/180.0 * p_rotation) - this->kLeafH/2 * cos(PI/180.0 * c_rotation) + p_h/4; // the last term is correction

            QString console_text="";
            for(int i=0; i<depth; i++) console_text+="    ";

            console_text += "└ FILE: " + child_file_name_list[i] + ", x: " + QString::number(c_x) + ", y: " + QString::number(c_y);

            /* Output to debug console */
            qDebug()<<console_text;

            /* Output to user console */
            emit console_write(console_text);

            /* Draw a file leaf */
            emit draw_file(root_name+dir_name+"/"+child_file_name_list[i], c_x, c_y, this->kLeafW, this->kLeafH, c_rotation);

            c_rotation += diff_rotation;
        }

    }
}

void TreeGenerator::setAbort(bool abort)
{
    this->abort = abort;
}

QString TreeGenerator::getDirectoryPath()
{
    return directory_path;
}

QString TreeGenerator::getForeDirPath()
{
    return fore_dir_path;
}

QString TreeGenerator::getRootDir()
{
    return root_name;
}
