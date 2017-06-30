//ARQUIVO NO QT

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowdir.h"
#include "aviso.h"
#include <dirent.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <QColor>
#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close

#define MAXMSG 80
#define MAXDIR 70
#define PORTNUM 4325

class Comando {
    public:
        char msg[MAXMSG];
        int comando;
};

struct End {
    bool isDiretorio;
    char endereco[MAXMSG];
};

struct TamEnd {
    int tamanho;
    char endereco_d[MAXMSG];
};




//Função que lista os diretorios na tela
void MainWindow::listarArquivos(std::string nomeDir)
{
        QString s = nomeDir.c_str();
        ui->dirFiles->setText(s)
                ;
        struct sockaddr_in endereco;
        int socketId;

        Comando cmd;
        strcpy(cmd.msg,nomeDir.c_str());
        cmd.comando = 1;

        int bytesenviados;
        int byteslidos;

        //Configurações do endereço
        memset(&endereco, 0, sizeof(endereco));
        endereco.sin_family = AF_INET;
        endereco.sin_port = htons(PORTNUM);
        endereco.sin_addr.s_addr = inet_addr("192.168.7.2");
        //endereco.sin_addr.s_addr = inet_addr("127.0.0.1");

        //Criando o Socket
        socketId = socket(AF_INET, SOCK_STREAM, 0);

        //Verificar erros
        if (socketId == -1)
        {
            printf("Falha ao executar socket()\n");
            exit(EXIT_FAILURE);
        }

        //Conectando o socket cliente ao socket servidor
        if ( ::connect (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
        {
            printf("Falha ao executar connect()\n");
            exit(EXIT_FAILURE);
        }
        printf ("Cliente conectado ao servidor\n");

        printf("Cliente vai enviar uma mensagem\n");
        bytesenviados = send(socketId,&cmd,sizeof(cmd),0);

        if (bytesenviados == -1)
        {
            printf("Falha ao executar send()");
            exit(EXIT_FAILURE);
        }
        printf("Cliente enviou uma msg \n\n");

        //RECEBENDO RESP
        End diretorios[MAXDIR];
        printf("Servidor vai ficar esperando uma mensagem\n");

        byteslidos = recv(socketId,diretorios,sizeof(diretorios),0);

        if (byteslidos == -1) {
                printf("Falha ao executar recv()");
                exit(EXIT_FAILURE);
        } else if (byteslidos == 0) {
                printf("Cliente finalizou a conexão\n");
                exit(EXIT_SUCCESS);
        }

        printf("Mensagem recebida \n");
        ::close(socketId);

        int i = 0;
        int d = 0;
        while (strcmp(diretorios[i].endereco, "--") != 0) {
            if (diretorios[i].isDiretorio == true) {
                //Cria um item
                QListWidgetItem *itm = new QListWidgetItem;
                itm->setText(diretorios[i].endereco);
                itm->setIcon(QIcon("/home/joao/folder_icon.png"));
                itm->setBackgroundColor(QColor(255, 255, 255, 255));

                //Adiciona item na lista
                ui->listWidget->insertItem(0, itm);
                d++;
            } else {
                //Cria um item
                QListWidgetItem *itm = new QListWidgetItem;
                itm->setText(diretorios[i].endereco);
                itm->setIcon(QIcon("/home/joao/file_icon.png"));
                itm->setBackgroundColor(QColor(254, 254, 254, 255));

                //Adiciona item na lista
                ui->listWidget->insertItem(d, itm);
            }
            i++;
        }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Inicializa com a pasta do usuário
    diretorio.push_back("/home/debian/");
    listarArquivos(diretorio[0]);
    ui->listWidget->setCurrentRow(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Função ativada quando o botão de Download é clicado
void MainWindow::on_pushButton_3_clicked()
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
            strcat (string, " ");
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
            strcat (string, " ");
            pch = strtok (NULL, " ");
       }
       string[(strlen(string)-1)] = '\0';
    }

    strcat (comando, "root@192.168.7.2:\"'");
    strcat (comando, string);
    strcat (comando, "'\" /home/joao/Downloads/");
    std::cout << comando << "\n";
    system(comando);
    Aviso aviso;
    aviso.setModal(true);
    aviso.exec();

    /*
    char cmd[1024] = "";
    strcat (cmd, "python3 /home/joao/Área\\ de\\ Trabalho/proj_so/correioeletronico.py ");
    strcat (cmd, "'um arquivo/diretorio foi baixado da BeagleBone Black'");
    std::cout << cmd << "\n";
    system (cmd);
    */
}

//Função ativada quando o botão de Abrir é clicado
void MainWindow::on_pushButton_2_clicked()
{
    if ( (ui->listWidget->count() != 0) && (ui->listWidget->currentItem()->backgroundColor() == QColor(255, 255, 255, 255)) ) {
        //Recebe o valor do item selecionado
        std::string s = ui->listWidget->currentItem()->text().toStdString();
        while(ui->listWidget->count() > 0)
        {
          //Apaga itens da lista
          ui->listWidget->takeItem(0);
        }
        diretorio.push_back(s + "/");

        std::string dirA;
        for (unsigned int i = 0; i < diretorio.size(); i++) {
            dirA = dirA + diretorio[i];
        }

        //Solicita as pastas/arquivo de acordo com o novo endereço passado
        listarArquivos(dirA);
        if (ui->listWidget->count() != 0) {ui->listWidget->setCurrentRow(0);}

        /*
        char cmd[1024] = "";
        strcat (cmd, "python3 /home/joao/Área\ de\ Trabalho/proj_so/correioeletronico.py ");
        strcat (cmd, "'um arquivo/diretorio foi enviado para a BeagleBone Black'");
        system (cmd);
        */
    }
}

//Função ativada quando o botão de Voltar é clicado
void MainWindow::on_pushButton_4_clicked()
{
    //Se não estiver na pasta inicial (do usuário)
    if (diretorio.size() > 1) {
        if (ui->listWidget->count() != 0) {
            while(ui->listWidget->count() > 0)
            {
              //Apaga todos os itens da lista
              ui->listWidget->takeItem(0);
            }
        }
        //Apago ultimo elemento do vetor de diretorios
        diretorio.pop_back();
        std::string dirA;
        for (unsigned int i = 0; i < diretorio.size(); i++) {
            dirA = dirA + diretorio[i];
        }
        //Solicita as pastas/arquivo de acordo com o novo endereço passado
        listarArquivos(dirA);
        ui->listWidget->setCurrentRow(0);
    }
}

void MainWindow::on_pushButton_clicked()
{
    std::string dirA;
    for (unsigned int i = 0; i < diretorio.size(); i++) {
        dirA = dirA + diretorio[i];
    };
    WindowDir w_dir(dirA, true);
    w_dir.setModal(true);
    w_dir.exec();
    while(ui->listWidget->count() > 0)
    {
      //Apaga todos os itens da lista
      ui->listWidget->takeItem(0);
    }
    listarArquivos(dirA);
    ui->listWidget->setCurrentRow(0);
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    on_pushButton_2_clicked();
}

//Deleta pasta/arquivo
void MainWindow::on_pushButton_5_clicked()
{
    std::string dirA;
    for (unsigned int i = 0; i < diretorio.size(); i++) {
        dirA = dirA + diretorio[i];
    }

    char comando[1024] = "";
    char string[1024] = "";
    char * pch;

    if (ui->listWidget->currentItem()->backgroundColor() == QColor(255,255,255,255)) {
        strcat (comando, "rm -rf ");
        dirA = dirA + ui->listWidget->currentItem()->text().toStdString() + "/";
        char * diretorio = new char[dirA.size() + 1];
        std::copy(dirA.begin(), dirA.end(), diretorio);
        diretorio[dirA.size()] = '\0';

        pch = strtok (diretorio," ");
        while (pch != NULL)
        {
            strcat (string, pch);
            strcat (string, " ");
            pch = strtok (NULL, " ");
       }
       string[(strlen(string)-2)] = '\0';

    } else {
        strcat (comando, "rm ");
        dirA = dirA + ui->listWidget->currentItem()->text().toStdString();
        char * diretorio = new char[dirA.size() + 1];
        std::copy(dirA.begin(), dirA.end(), diretorio);
        diretorio[dirA.size()] = '\0';

        pch = strtok (diretorio," ");
        while (pch != NULL)
        {
            strcat (string, pch);
            strcat (string, " ");
            pch = strtok (NULL, " ");
       }
       string[(strlen(string)-1)] = '\0';
    }

    strcat (comando, string);

    struct sockaddr_in endereco;
    int socketId;

    Comando cmd;
    strcpy(cmd.msg,comando);
    cmd.comando = 3;

    int bytesenviados;
    int byteslidos;

    //Configurações do endereço
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    endereco.sin_addr.s_addr = inet_addr("192.168.7.2");
    //endereco.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Criando o Socket
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    //Verificar erros
    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    //Conectando o socket cliente ao socket servidor
    if ( ::connect (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
    {
        printf("Falha ao executar connect()\n");
        exit(EXIT_FAILURE);
    }
    printf ("Cliente conectado ao servidor\n");

    printf("Cliente vai enviar uma mensagem\n");
    bytesenviados = send(socketId,&cmd,sizeof(cmd),0);

    if (bytesenviados == -1)
    {
        printf("Falha ao executar send()");
        exit(EXIT_FAILURE);
    }
    printf("Cliente enviou uma msg \n\n");

    std::string dirA2;
    for (unsigned int i = 0; i < diretorio.size(); i++) {
        dirA2 = dirA2 + diretorio[i];
    }

    while(ui->listWidget->count() > 0)
    {
      //Apaga itens da lista
      ui->listWidget->takeItem(0);
    }

    listarArquivos(dirA2);
    ui->listWidget->setCurrentRow(0);
    if (ui->listWidget->count() != 0) {ui->listWidget->setCurrentRow(0);}

    /*
    char cmd2[1024] = "";
    strcat (cmd2, "python3 /home/joao/Área\ de\ Trabalho/proj_so/correioeletronico.py ");
    strcat (cmd2, "'um arquivo/diretorio foi apagado da BeagleBone Black'");
    system (cmd2);
    */
}




void MainWindow::on_actionArmazenamento_triggered()
{

}
