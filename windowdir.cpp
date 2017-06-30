#include "windowdir.h"
#include "ui_windowdir.h"
#include <dirent.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <QColor>

WindowDir::WindowDir(std::string p, bool isD) :
    ui(new Ui::WindowDir), pasta(p), isDir(isD)
{
    ui->setupUi(this);
    diretorio.push_back("/home/joao/");
    listarArquivos(diretorio[0]);
    ui->listWidget->setCurrentRow(0);
}

WindowDir::~WindowDir()
{
    delete ui;
}


void WindowDir::listarArquivos(std::string nomeDir)
{
    QString s = nomeDir.c_str();
    ui->dirFiles->setText(s);
    DIR *dir = 0;
    struct dirent *entrada = 0;
    unsigned char isDir = 0x4;
    unsigned char isFile = 0x8;

    dir = opendir(nomeDir.c_str());

    if (dir == 0) {
        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
        exit (1);
    }

    //Iterar sobre o diretorio
    int d = 0;
    while (entrada = readdir(dir)) {
        std::string s = "..";
        std::string s2 = ".";
        if (entrada->d_name[0] != '.') {
            if (entrada->d_type == isFile) {
                //Create Our Item
                QListWidgetItem *itm = new QListWidgetItem;
                itm->setText(entrada->d_name);
                itm->setIcon(QIcon("/home/joao/file_icon.png"));
                itm->setBackgroundColor(QColor(254, 254, 254, 255));

                //Add Our Item To Our List
                ui->listWidget->insertItem(d, itm);
            } else if (entrada->d_type == isDir) {
                d++;
                //Create Our Item
                QListWidgetItem *itm = new QListWidgetItem;
                itm->setText(entrada->d_name);
                itm->setIcon(QIcon("/home/joao/folder_icon.png"));
                itm->setBackgroundColor(QColor(255, 255, 255, 255));

                //Add Our Item To Our List
                ui->listWidget->insertItem(0, itm);
            }
        }
    }
    closedir (dir);
}

void WindowDir::on_pushButton_2_clicked()
{
    if ( (ui->listWidget->count() != 0) && (ui->listWidget->currentItem()->backgroundColor() == QColor(255,255,255,255)) ) {
        std::string s = ui->listWidget->currentItem()->text().toStdString();
        while(ui->listWidget->count() > 0)
        {
          ui->listWidget->takeItem(0);
        }
        diretorio.push_back(s + "/");
        std::string dirA;
        for (unsigned int i = 0; i < diretorio.size(); i++) {
            dirA = dirA + diretorio[i];
        }
        listarArquivos(dirA);
        ui->listWidget->setCurrentRow(0);
    }
}

void WindowDir::on_pushButton_4_clicked()
{
    if ( diretorio.size() > 1) {
        if (ui->listWidget->count() != 0) {
            while(ui->listWidget->count() > 0)
            {
              ui->listWidget->takeItem(0);
            }
        }

        std::string dirA;
        diretorio.pop_back();
        for (unsigned int i = 0; i < diretorio.size(); i++) {
            dirA = dirA + diretorio[i];
        }
        listarArquivos(dirA);
        ui->listWidget->setCurrentRow(0);
    }
}

void WindowDir::on_listWidget_doubleClicked(const QModelIndex &index)
{
    on_pushButton_2_clicked();
}

void WindowDir::on_pushButton_clicked()
{
    std::string dirA;
    for (unsigned int i = 0; i < diretorio.size(); i++) {
        dirA = dirA + diretorio[i];
    }
    char comando[1024] = "";
    char string[1024] = "";
    char * pch;

    if (ui->listWidget->currentItem()->backgroundColor() == QColor(255,255,255,255)) {
        strcat (comando, "scp -r ");
        dirA = dirA + ui->listWidget->currentItem()->text().toStdString() + "/";
        char * diretorio = new char[dirA.size() + 1];
        std::copy(dirA.begin(), dirA.end(), diretorio);
        diretorio[dirA.size()] = '\0';

        pch = strtok (diretorio," ");
        while (pch != NULL)
        {
            strcat (string, pch);
            strcat (string, "\\ ");
            pch = strtok (NULL, " ");
       }
       string[(strlen(string)-2)] = '\0';

    } else {
        strcat (comando, "scp ");
        dirA = dirA + ui->listWidget->currentItem()->text().toStdString();
        char * diretorio = new char[dirA.size() + 1];
        std::copy(dirA.begin(), dirA.end(), diretorio);
        diretorio[dirA.size()] = '\0';

        pch = strtok (diretorio," ");
        while (pch != NULL)
        {
            strcat (string, pch);
            strcat (string, "\\ ");
            pch = strtok (NULL, " ");
       }
       string[(strlen(string)-2)] = '\0';
    }

    char string2[1024] = "";
    char * pch2;
    char * diretorio2 = new char[pasta.size() + 1];
    std::copy(pasta.begin(), pasta.end(), diretorio2);
    diretorio2[pasta.size()] = '\0';

    pch2 = strtok (diretorio2," ");
    while (pch2 != NULL)
    {
        strcat (string2, pch2);
        strcat (string2, " ");
        pch2 = strtok (NULL, " ");
   }
   string2[(strlen(string2)-2)] = '\0';

   strcat (comando, string);
   strcat (comando, " ");
   strcat (comando, "root@192.168.7.2:\"'");
   strcat (comando, string2);
   strcat (comando, "'\"");
   system(comando);
   this->close();
}
