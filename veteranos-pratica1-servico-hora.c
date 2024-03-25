#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
#include <unistd.h>
#include <time.h>

int main() {
    struct sockaddr_in info;
    int socket_entrada, socket_conexao;
    int tamanho_estrutura_socket;
    time_t data_e_hora;
    char* data_e_hora_por_extenso;
    data_e_hora_por_extenso = malloc(26 * sizeof(char));
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(
        wVersionRequested,
        &wsaData
    );
    socket_entrada = socket(AF_INET, SOCK_STREAM, 6);
#else
    socket_entrada = socket(AF_INET, SOCK_STREAM, 0);
#endif
    if (socket_entrada < 0) {
        printf("Vixe!\n");
        exit(1);
    }
    info.sin_family = AF_INET;
    info.sin_port = htons(7658);
    info.sin_addr.s_addr = INADDR_ANY;
    tamanho_estrutura_socket = sizeof(info);

    if (bind(socket_entrada, (struct sockaddr*)&info, sizeof(info)) == 0) {
        printf("%s", inet_ntoa(info.sin_addr));
        listen(socket_entrada, 5);
        while (1) {
            socket_conexao = accept(socket_entrada, (struct sockaddr*)&info, &tamanho_estrutura_socket);
            data_e_hora = time(NULL);
            data_e_hora_por_extenso = ctime(&data_e_hora);
            printf("Cliente Conectado! Data e hora: %s\n", data_e_hora_por_extenso);
            write(socket_conexao, data_e_hora_por_extenso, 25);
            close(socket_conexao);
        }
        close(socket_entrada);
    } else {
        printf("Vixe! Impossivel usar o endereco.\n"); exit(1);
    }
    return 0;
}
