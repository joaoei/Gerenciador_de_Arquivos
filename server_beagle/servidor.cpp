#include <cstdio>       //printf
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <dirent.h>
#include <string>
#include <iostream>

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
    char endereco_d[MAXMSG];
};

struct TamEnd {
    int tamanho;
    char endereco_d[MAXMSG];
};

int main(int argc, char *argv[])
{
    //variáveis do servidor
    struct sockaddr_in endereco;
    int socketId;

    //variáveis relacionadas com as conexões clientes
    struct sockaddr_in enderecoCliente;
    socklen_t tamanhoEnderecoCliente = sizeof(struct sockaddr);
    int conexaoClienteId;

    Comando cmd;
    int byteslidos;
    int bytesenviados;

    /*
     * Configurações do endereço
    */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    //endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_addr.s_addr = inet_addr("192.168.7.2");
    //endereco.sin_addr.s_addr = inet_addr("127.0.0.1");


    socketId = socket(AF_INET, SOCK_STREAM, 0);

    //Verificar erros
    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    //Conectando o socket a uma porta. Executado apenas no lado servidor
    if ( bind (socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr)) == -1 )
    {
        printf("Falha ao executar bind()\n");
        exit(EXIT_FAILURE);
    }

    //Habilitando o servidor a receber conexoes do cliente
    if ( listen( socketId, 10 ) == -1)
    {
        printf("Falha ao executar listen()\n");
        exit(EXIT_FAILURE);
    }

    //servidor ficar em um loop infinito
    while(1)
    {

        printf("\nServidor: esperando conexões clientes\n");

        //Servidor fica bloqueado esperando uma conexão do cliente
        conexaoClienteId = accept( socketId,(struct sockaddr *) &enderecoCliente,&tamanhoEnderecoCliente );

        printf("Servidor: recebeu conexão de %s\n", inet_ntoa(enderecoCliente.sin_addr));

        //Verificando erros
        if ( conexaoClienteId == -1)
        {
            printf("Falha ao executar accept()");
            exit(EXIT_FAILURE);
        }

        //receber uma msg do cliente
        printf("Servidor vai ficar esperando uma mensagem\n");
        byteslidos = recv(conexaoClienteId,&cmd,sizeof(cmd),0);

        if (byteslidos == -1)
        {
            printf("Falha ao executar recv()");
            exit(EXIT_FAILURE);
        }
        else if (byteslidos == 0)
        {
            printf("Cliente finalizou a conexão\n");
            exit(EXIT_SUCCESS);
        }

        printf("Servidor recebeu mensagem \n\n");

        //Variáveis usadas no listamento de arquivos
        DIR *dir = 0;
		struct dirent *entrada = 0;
		unsigned char isDir = 0x4;
		unsigned char isFile = 0x8;
		int j = 0;

		End diretorios[MAXDIR];
        
        //Vetor com structs (tamanho e nome do diretorio)
        TamEnd TamDiretorios[MAXDIR];

        switch (cmd.comando) {
        	case 1: 
			    dir = opendir(cmd.msg);

			    if (dir == 0) {
			        std::cerr << "Nao foi possivel abrir diretorio." << std::endl;
			        exit (1);
			    }

			    //Iterar sobre o diretorio
			    while (entrada = readdir(dir)) {
			        if (entrada->d_name[0] != '.') {
			            if (entrada->d_type == isFile) { 
			            	//Se é arquivo, adiciona o nome no vetor e seta o valor do isDiretorio como false
			            	strcpy(diretorios[j].endereco_d, entrada->d_name);
			            	diretorios[j].isDiretorio = false;
			            } else if (entrada->d_type == isDir) {
			            	//Se é diretorio, adiciona o nome no vetor e seta o valor do isDiretorio como true
			                strcpy(diretorios[j].endereco_d, entrada->d_name);
			            	diretorios[j].isDiretorio = true;
			            }
			        	j++;
			        }
			    }
			    closedir (dir);
			    //Adiciona um endereço com nome vazio, que significa o ponto de parada
			    strcpy(diretorios[j].endereco_d, "--");
        		diretorios[j].isDiretorio = false;

        		//Responde o cliente com o vetor de diretorios/arquivos
		    	bytesenviados = send(conexaoClienteId,diretorios,sizeof(diretorios),0);

		    	if (bytesenviados == -1)
			    {
			        printf("Falha ao executar send()");
			        exit(EXIT_FAILURE);
			    }
			    printf("Servidor enviou uma msg \n");
        		break;
        	case 2:
                // Executar o comando para o diretorio passado pelo socket
        		//system (s); s = du cmd.msg --max-depth=1 -h > relatorio

                //Abrir o arquivo salvando o tamanho e o diretorio
                //Se atentar ao tamanho que pode vir com K, M ou G
                //Salvar no vetor TamDiretorios
                //Depois de adicionar todos, colocar um "vazio" com nome "--" e tamanho 0
                //Enviar o vetor ao cliente 
        		break;
        	case 3:
        		system(cmd.msg);
        		break;
        }
        close(conexaoClienteId);
    }

    return 0;
}
