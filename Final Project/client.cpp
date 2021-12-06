#include <iostream>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

#define MAX_LEN 256

using namespace std;

// Exit flag
bool flag_exit = false;

// protype functions
void recv_message(int client_socket);
void send_message(int client_socket);

int main() {

    struct sockaddr_in client;

    // Fail if not socket doesn't connect
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket: ");
        exit(-1);
    }

    const int port = 10000;

    //Connect to server
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&client.sin_zero, 0);

    // Fail if can't connect
    if((connect(client_socket,(struct sockaddr *) &client, sizeof(struct sockaddr_in))) == -1) {
        perror("Connection: ");
        exit(-1);
    }

    // user enters name & sends to client_socket     
    char name[MAX_LEN];
    cout << "Enter your name: ";
    cin.getline(name, MAX_LEN);
    send(client_socket, name, sizeof(name), 0);

    // Connected to chat room message
    cout << " --------- Welcome to the Chat Room --------- " << end;

    // Send & Receive threads
    thread sendThread(send_message, client_socket);
    thread recvThread(recv_message, client_socket);

    // send thread
    if(sendThread.joinable())
        sendThread.join();  
    
    // recieve thread
    if(recvThread.joinable())
        recvThread.join();

    // Another way of doing it is below

    //// Move threads
    // tsend = move(sendThread);
    // trecv = move(recvThread);

    //// join threads
    // if(tsend.joinable())
    //     tsend.join();

    // if(trecv.joinable())
    //     trecv.join();

    return 0;
}

// Receives messages
void recv_message(int client_socket) 
{
    while(1) 
    {
        if(flag_exit)
            return;

        char name[MAX_LEN], str[MAX_LEN];
        int bytes_received = recv(client_socket, name, sizeof(name), 0);
        if(bytes_received <= 0)
            continue;

        recv(client_socket, str, sizeof(str), 0);
        cout << "You: ";
        fflush(stdout);
    }
}

// Send messages
void send_message(int client_socket) 
{
    while(1) 
    {
        cout << "You: "
        char str[MAX_LEN]
        cin.getline(str, MAX_LEN);
        send(client_socket, str, sizeof(str), 0);

        // Exit phrase
        if(stcrmp(str, "!exit") == 0) 
        { 
            flag_exit = true;
            trecv.detach();
            close(client_socket);
            return;
        }

        // if want to we can also add an @ reference for private messaging
        // also another idea is if they send a !color then they can change their color of their name in the chat
    }
}