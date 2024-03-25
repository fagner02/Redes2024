#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]) {
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
    struct addrinfo* result = NULL, hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int n = getaddrinfo(argv[1], argv[2], &hints, &result);

    struct sockaddr_in* addr = (struct sockaddr_in*)result->ai_addr;
    printf("Connecting to IP %s on port: %d\n", inet_ntoa(addr->sin_addr), htons(addr->sin_port));

    char buffer[1000];

    if (connect(socket_entrada, result->ai_addr, result->ai_addrlen) == 0) {
        scanf("%1000[^\n]%*c", buffer);

        n = send(socket_entrada, buffer, 10, MSG_OOB);
        if (n == SOCKET_ERROR) {
            printf("error writing");
        }
        close(socket_entrada);
    } else {
        printf("Vixe! Impossivel usar o endereco.\n"); exit(1);
    }

    return 0;
}