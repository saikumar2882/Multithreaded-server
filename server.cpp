#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define NUM_COLORS 6
#define MAX_LEN 200

using namespace std;

struct endTerminal{
	int id;
	string clientName;
	int socket;
	thread th;
};

vector<endTerminal> clients;
string def_col="\033[0m"; //regular colors 
string colorsName[]={"\033[32m", "\033[33m", "\033[34m", "\033[31m", "\033[35m","\033[37m"};
int seed=0;
mutex cout_mutex,clients_mutex;

string color(int code);
void setName(int id, char clientName[]);
void sharedPrint(string str, bool endLine);
int broadcastMessage(string message, int sender_id);
int broadcastMessage(int num, int sender_id);
void endConnection(int id);
void handleClient(int clientSocket, int id);

int main(){
	int serverSocket;
	if((serverSocket=socket(AF_INET,SOCK_STREAM,0))==-1){perror("socket: ");exit(-1);}
	struct sockaddr_in server;

	server.sin_family=AF_INET;
	server.sin_port=htons(10000);
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);
	if((bind(serverSocket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1){perror("bind error: ");exit(-1);}
	if((listen(serverSocket,8))==-1){perror("listen error: ");exit(-1);}
	struct sockaddr_in client;
	int clientSocket;
	unsigned int len=sizeof(sockaddr_in);
	cout << colorsName[NUM_COLORS-1]<<"\n\t*********WELCOME TO CHAT-ROOM***********"<<"\n"<<def_col;
	while(true){
		if((clientSocket=accept(serverSocket,(struct sockaddr *)&client,&len))==-1){perror("accept error: ");exit(-1);}
		seed++;
		thread t(handleClient,clientSocket,seed);
		lock_guard<mutex> guard(clients_mutex);
		clients.push_back({seed, string("Anonymous"),clientSocket,(move(t))});
	}
	for(int i=0; i<clients.size(); i++){
		if(clients[i].th.joinable()) clients[i].th.join();
	}
	close(serverSocket);
	return 0;
}

string color(int code){return colorsName[code%NUM_COLORS];}

// Set name of client
void setName(int id, char clientName[]){
	for(int i=0; i<clients.size(); i++){
			if(clients[i].id==id) clients[i].clientName=string(clientName);		
	}	
}

// For synchronisation of cout statements
void sharedPrint(string str, bool endLine=true){	
	lock_guard<mutex> guard(cout_mutex);
	cout<<str;
	if(endLine) cout<<endl;
}

// Broadcast message to all clients except the sender
int broadcastMessage(string message, int sender_id){
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id) send(clients[i].socket,temp,sizeof(temp),0);
	}		
}

// Broadcast a number to all clients except the sender
int broadcastMessage(int num, int sender_id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id) send(clients[i].socket,&num,sizeof(num),0);
	}		
}

void endConnection(int id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==id){
			lock_guard<mutex> guard(clients_mutex);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handleClient(int clientSocket, int id){
	char clientName[MAX_LEN],str[MAX_LEN];
	recv(clientSocket,clientName,sizeof(clientName),0);
	setName(id,clientName);	

	// Display welcome message
	string welcome_message=string(clientName)+string(" has joined");
	broadcastMessage("#NULL",id);	
	broadcastMessage(id,id);								
	broadcastMessage(welcome_message,id);	
	sharedPrint(color(id)+welcome_message+def_col);
	
	while(true){
		int bytes_received=recv(clientSocket,str,sizeof(str),0);
		if(bytes_received<=0)
			return;
		if(strcmp(str,"#exit")==0){
			// Display leaving message
			string message=string(clientName)+string(" has left");		
			broadcastMessage("#NULL",id);			
			broadcastMessage(id,id);						
			broadcastMessage(message,id);
			sharedPrint(color(id)+message+def_col);
			endConnection(id);							
			return;
		}
		broadcastMessage(string(clientName),id);					
		broadcastMessage(id,id);		
		broadcastMessage(string(str),id);
		sharedPrint(color(id)+clientName+" : "+def_col+str);		
	}	
}