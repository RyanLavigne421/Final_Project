/*

NOTES:
This program was compiled using GCC version 11.2.0 and was found there
might be possbile complications with using a newer version of GCC

This project does successfully compile and work using GCC version 11.2.0
have not tested it on other versions.

*/

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#define MAX_LEN 200

using namespace std;

struct console
{
	int id;
	string name;
	int socket;
	thread th;
};

vector<console> clients;
int seed = 0;
int firstTime = 0;
socklen_t client_len;
mutex cout_mtx, clients_mtx;

void set_name(int id, char name[]);
void print_message(string str, bool endLine);
void send_message(string message, int sender_id);
void close_client(int id);
void handle_client(int client_socket, int id);
string encrypt_message(string message);


int main()
{
	int server_socket;

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 0);

    if((server_socket = socket(AF_INET,SOCK_STREAM,0)) == -1) 
	{
		perror("socket: ");
		exit(1);
	}

	if((bind(server_socket, (struct sockaddr *) &server, sizeof(struct sockaddr_in))) == -1)
	{
		perror("bind error: ");
		exit(1);
	}

	if((listen(server_socket, 8)) == -1)
	{
		perror("listen error: ");
		exit(1);
	}

	struct sockaddr_in client;
	int client_socket;
	client_len = sizeof(sockaddr_in);

	cout<<"\n\t --------- Welcome to the Chat Room --------- "<<endl;

	while(1)
	{
		if((client_socket = accept(server_socket, (struct sockaddr *) &client, &client_len)) == -1)
		{
			perror("accept error: ");
			exit(1);
		}
		
		thread t(handle_client, client_socket, seed);
        lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Anonymous"), client_socket, (move(t))});
        seed++;
	}

	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

// Set name of client
void set_name(int id, char name[])
{
	for(int i = 0; i < clients.size(); i++)
	{
			if(clients[i].id == id)	
			{
				clients[i].name = string(name);
			}
	}	
}

// For synchronisation of cout statements
void print_message(string str, bool endLine=true)
{	
    lock_guard<mutex> guard(cout_mtx);
	cout << str;
	if(endLine)
		cout << endl;
}

// Broadcast message to all clients except the sender
void send_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp, message.c_str());

	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id)
		{
			send(clients[i].socket, temp, sizeof(temp), 0);
		}
	}	
}

void close_client(int id)
{
	for(int i=0; i < clients.size(); i++)
	{
		if(clients[i].id == id)	
		{
            lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin() + i);
			close(clients[i].socket);
			break;
		}
	}
}

void handle_client(int client_socket, int id)
{
	char name[MAX_LEN], str[MAX_LEN];
    
	recv(client_socket, name, sizeof(name), 0);
	set_name(id, name);


	// Display welcome message
	string welcome_message = string(name) + string(" has joined");
	send_message("!NULL", id);		
	string encrypt_msg = encrypt_message(welcome_message);			
	send_message(encrypt_msg, id);	
	print_message(welcome_message);
	
	while(1)
	{
		int bytes_received = recv(client_socket, str, sizeof(str), 0);
		if(bytes_received <= 0)
			return;

		if(strcmp(str, "!EXIT") == 0)
		{
			// Display leaving message
			string leaving = string(name) + string(" has left");
			string encrypt_leave = encrypt_message(leaving);						
			send_message(encrypt_leave, id);
			print_message(leaving);
			close_client(id);							
			return;
		}
		send_message(string(name), id);						
		send_message(string(str), id);
        if(firstTime != 0)
		    print_message(string(name) + ": "  + string(str));	
            
        firstTime++;	
	}	
}

string encrypt_message(string message)
{
    char key = '7';
    char temp;
    char data;
    string retVal = "";

    //cout << message << endl;

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



         