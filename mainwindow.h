#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void listarArquivos(std::string n);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_actionArmazenamento_triggered();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> diretorio;
};

#endif // MAINWINDOW_H
