#include <bits/stdc++.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>

#define NUM_COLORS 6
#define MAX_LEN 200

using namespace std;

bool exit_flag=false;
thread thread_send, thread_recieve;
int clientSocket;
string def_col="\033[0m";
string colorsName[]={"\033[32m", "\033[33m", "\033[34m", "\033[31m", "\033[35m","\033[37m"};

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void sendMessage(int clientSocket);
void recvMessage(int clientSocket);

int main()
{
	if((clientSocket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(10000); // Port no. of server
	client.sin_addr.s_addr=INADDR_ANY;
	bzero(&client.sin_zero,0);

	if((connect(clientSocket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	signal(SIGINT, catch_ctrl_c);
	char clientName[MAX_LEN];
	cout<<"Enter Your Name : ";
	cin.getline(clientName,MAX_LEN);
	send(clientSocket,clientName,sizeof(clientName),0);

	cout << colorsName[NUM_COLORS-1]<<"\n\t*********CHAT ROOM***********"<<"\n"<<def_col;

	thread t1(sendMessage, clientSocket);
	thread t2(recvMessage, clientSocket);

	thread_send=move(t1);
	thread_recieve=move(t2);

	if(thread_send.joinable())
		thread_send.join();
	if(thread_recieve.joinable())
		thread_recieve.join();
			
	return 0;
}

// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="#exit";
	send(clientSocket,str,sizeof(str),0);
	exit_flag=true;
	thread_send.detach();
	thread_recieve.detach();
	close(clientSocket);
	exit(signal);
}

string color(int code)
{
	return colorsName[code%NUM_COLORS];
}

// Erase text from end terminal

int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<back_space;
	}	
}

// Send message to everyone in the server

void sendMessage(int clientSocket)
{
	while(1)
	{
		cout<<colorsName[1]<<"You : "<<def_col;
		char str[MAX_LEN];
		cin.getline(str,MAX_LEN);
		send(clientSocket,str,sizeof(str),0);
		if(strcmp(str,"#exit")==0)
		{
			exit_flag=true;
			thread_recieve.detach();	
			close(clientSocket);
			return;
		}	
	}		
}

// Receive message from the server to client 

void recvMessage(int clientSocket)
{
	while(1)
	{
		if(exit_flag)
			return;
		char clientName[MAX_LEN], str[MAX_LEN];
		int color_code;
		int bytes_received=recv(clientSocket,clientName,sizeof(clientName),0);
		if(bytes_received<=0)
			continue;
		recv(clientSocket,&color_code,sizeof(color_code),0);
		recv(clientSocket,str,sizeof(str),0);
		eraseText(6);
		if(strcmp(clientName,"#NULL")!=0)
			cout<<color(color_code)<<clientName<<" : "<<def_col<<str<<endl;
		else
			cout<<color(color_code)<<str<<endl;
		cout<<colorsName[1]<<"You : "<<def_col;
		fflush(stdout);
	}	
}
