/*

NOTES:
This program was compiled using GCC version 11.2.0 and was found there
might be possbile complications with using a newer version of GCC

This project does successfully compile and work using GCC version 11.2.0
have not tested it on other versions.

*/

#include <iostream>
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

int main() {

    struct sockaddr_in client;

    const int port = 10000;

    //Connect to server
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&client.sin_zero, 0);

    // Fail if not socket doesn't connect
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket: ");
        exit(1);
    }

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

    // Move threads
    thread_send = move(sendThread);
    thread_recv = move(recvThread);

    // join threads
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
        string decrypted = decrypt_message(str);
        eraseText(6);


        if(strcmp(name, "!NULL") != 0) {
            cout << name << ": " << decrypted << endl;
        } else {
            cout << decrypted << endl;
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
        char char_message[MAX_LEN];
        char temp[MAX_LEN];
        string encrypted_msg;
        string input;
        getline(cin, input);
        encrypted_msg = encrypt_message(input);
        strcpy(char_message, encrypted_msg.c_str());
        send(client_socket, char_message, sizeof(char_message), 0);

        // Exit phrase
        if(strcmp(char_message, "!EXIT") == 0) 
        { 
            flag_exit = true;
            thread_recv.detach();
            close(client_socket);
            return;
        }
    }
}

string encrypt_message(string message)
{
    char key = '7';
    char temp;
    char data;
    string retVal = "";

    for (int i = 0; i < message.length(); i++)
    {
        temp = message[i];
        // edit the key itself
        asm("movb %[key], %%bl \n "
            "movb %[input], %%al \n "
            "xor $11001100, %%bl \n"
            "ror $3, %%bl \n"
            "addb %%bl, %%al \n "
            "ror $2, %%al \n"
            "xor $01011010, %%al \n"
            "ror $2, %%al \n "
            "not %%al \n "
            "xor $10101010, %%al \n "
            "movb %%al, %[output] \n "
            : [output] "+m"(data)               // output operands
            : [input] "m"(temp), [key] "m"(key) // input operands
            : "%bl", "%al");                    // clobbers
        retVal += data;
    }
    return retVal;
}

string decrypt_message(string message)
{
    char key = '7';
    char temp;
    char data;
    string retVal = "";

    for (int i = 0; i < message.length(); i++)
    {
        temp = message[i];
        asm("movb %[key], %%bl \n "
            "movb %[input], %%al \n "
            "xor $11001100, %%bl \n" // XOR KEY
            "ror $3, %%bl \n" // ROTATE KEY
            "xor $10101010, %%al \n "
            "not %%al \n"
            "rol $2, %%al \n "
            "xor $01011010, %%al \n"
            "rol $2, %%al \n "
            "subb %%bl, %%al \n"
            "movb %%al, %[output]\n"
            : [output] "+m"(data)               // output operands
            : [input] "m"(temp), [key] "m"(key) // input operands
            : "%bl", "%al");                    // clobbers
        retVal += data;
    }
    return retVal;
}
