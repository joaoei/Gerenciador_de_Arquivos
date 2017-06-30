#ifndef WINDOWDIR_H
#define WINDOWDIR_H

#include <QDialog>
#include <string>
#include <vector>

namespace Ui {
class WindowDir;
}

class WindowDir : public QDialog
{
    Q_OBJECT

public:
    WindowDir(std::string p, bool isD);
    ~WindowDir();
    void listarArquivos(std::string n);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::WindowDir *ui;
    std::vector<std::string> diretorio;
    std::string pasta;
    bool isDir;
};

#endif // WINDOWDIR_H
