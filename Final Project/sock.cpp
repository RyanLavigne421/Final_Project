#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


using namespace std;

int main(int arc, char const *argv[]) {
    
    // socket(int domain, type, protocol)
    // bind (fd, sockaddr, addr length)
    // listen(fd, backlog queue size)
    // accept(fd, addr, length)
    // connect(fd, addr, length)
    // send(fd, buffer, size, flags)
    // recieve(fd, buffer, size, flags)

    int sock_fd, new_sock, port;
    //sock length
    socklen_t client_len;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;
    int n;

    // socket(int domain, type, protocol)
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0 ) {
        perror("Error opening socket");
        exit(1);
    }

    port = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = 'localhost';
    // converting port number into network byte
    server_addr.sin_port = htons(port);

    // bind (fd, sockaddr, addr length)
    if(bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding");
        exit(1);
    }
        // listen(fd, backlog queue size)
    listen(sock_fd, 5);

    client_len = sizeof(client_addr);
    
    new_sock = accept(sock_fd, (struct sockaddr *) &client_addr, &client_len);

    if(new_sock < 0) {
        perror("Error on accept");
        exit(1);
    }
    // need to insert connected message print out
    bzero(buffer, 256);

    n = read(new_sock, buffer, 256);
    if (n < 0) {
        perror("Error reading from socket");
    }
    printf("Message: %s\n", buffer);

    close(new_sock);
    close(sock_fd);
    return 0;
}
