#include <iostream>
// #include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

#define MAX_LEN 200

using namespace std;

// Exit flag
bool flag_exit = false;

// Initializing threads
thread thread_send, thread_recv;

// Initializing client socket
int client_socket;

// Protoypes functions
void recv_message(int client_socket);
void send_message(int client_socket);
void eraseText(int count);
string encrypt_message(string message);
string decrypt_message(string message);
int generateKey();

int main() {

    struct sockaddr_in client;

    // Fail if not socket doesn't connect
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket: ");
        exit(1);
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
        exit(1);
    }

    // user enters name & sends to client_socket     
    char name[MAX_LEN];
    cout << "Enter your name: ";
    cin.getline(name, MAX_LEN);
    send(client_socket, name, sizeof(name), 0);

    // Connected to chat room message
    cout << " --------- Welcome to the Chat Room --------- " << endl;

    // Send & Receive threads
    thread sendThread(send_message, client_socket);
    thread recvThread(recv_message, client_socket);

    //// Move threads
    thread_send = move(sendThread);
    thread_recv = move(recvThread);

    //// join threads
    if(thread_send.joinable())
        thread_send.join();

    if(thread_recv.joinable())
        thread_recv.join();

    return 0;
}

//erases text from terminal
void eraseText(int count) {
    char back_space = 8;
    for(int i =  0; i < count; i++) {
        cout << back_space;
    }
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
        eraseText(6);

        if(strcmp(name, "!NULL") != 0) {
            cout << name << ": " << str << endl;
        } else {
            cout << str << endl;
        }

        cout << "You: ";
        fflush(stdout);
    }
}

// Send messages
void send_message(int client_socket) 
{
    while(1) 
    {
        cout << "You: ";
        char str[MAX_LEN];
        cin.getline(str, MAX_LEN);
        send(client_socket, str, sizeof(str), 0);

        // Exit phrase
        if(strcmp(str, "!exit") == 0) 
        { 
            flag_exit = true;
            thread_recv.detach();
            close(client_socket);
            return;
        }

        // if want to we can also add an @ reference for private messaging
        // also another idea is if they send a !color then they can change their color of their name in the chat
    }
}

string encrypt_message(string message, int key) {
    char temp;
    char data;
    string retVal = "";

    for(int i = 0; i < message.length(); i++) {
        temp = message[i];
        __asm {
            xor al, al // clearing al
            mov al, temp
            ror al, 1
            xor al, 01011010
            ror al, 2 
            not al
            xor al, 10101010
            mov data, al
        };
        retVal += data;
    }
    cout << "Encrypted: " << retVal << endl;
    return retVal;
}

string decrypt_message(string message, int key) {
    char temp;
    char data;
    string retVal = "";

    for(int i = 0; i < message.length(); i++) {
        temp = message[i];
        __asm {
            xor al, al
            mov al, temp
            xor al, 10101010
            not al
            rol al, 2
            xor al, 01011010
            rol al, 1
            mov data, al
        };
        retVal += data;
    }
    return retVal;
}

int generateKey() {
    int key = rand() + 10;
    return key;
}