# Multithreaded-server
Chat-room using socket programming and multi-threading in C++


# Project Title:

Chatroom using socket programming and multi-threading in C++

# Project Description:
 The project Chat application implements a Client-Server model in a multithreaded chat server by sockets to communicate among multiple users.
 In this application multiple users can join the server and communicate with each other. To handle large data(Sending and recieving data) from the multiple users, the multithreading concept is used.
 
 In the server as the users enters the handling the data of every client is made run parellely in a thread. The proper synchronization of messages of clients is maintained by the mutex locks. The mutex locks ensure that the common data between the parallel executing threads is get modified only by one thread that present in the server.
 
The chat-room server andclient are run on terminal. The server has to be started using a port number for connections with the clients. Once the connection is done, the client enter the server through the port number. When the client enters the server, client has to provide a unique username. The entry of every new client is displayed on the server.

The clients that are joined in the server can chat with other clients. The chats of every clients are properly synchronizied using mutex lock.

# Working of project
The project consists of handling the multiple clients using thread function(POSIX thread), Linux thread synchronization and Socket programming.

Lets us discuss each point
: 
### working of Thread function:

In Linux Operating  system, c++ provides POSIX thread(pthread) standard API  for all thread related functions. It allow us to create multiple threads for concurrent process flow. The execute the threads we need to include **-lpthread** in the command line while compiling the file. A thread is a sub-process that runs a set of commands individually of any other thread. So, every time a user connects to the server, a separate thread is created for that user, and communication from the server to the client takes place along individual threads based on port number created for the sake of the identity of each client.

* Mutex lock for Thread Synchronization 

    To Achieve the synchronization(To avoid the race condition) between the multiple threads(clients), we use a mutex. TheMutex is a lock that we set before using a shared resource and release after using it.
    
    when the lock is set, no other thread can access the locked region of code(Critical section). If the other thread try to access the critical section, the mutex lock allows the thread to sleep and the released when the critical section is free.

### working of socket programming
Socket programming is a way of connecting two nodes(one is server and another is client) on a network to communicate with each other. 

* **Stages for server**

    1.Socket creation: int sockfd = socket(domain, type, protocol)
    
    2.SetSockopt: helps in manipulating options for the socket referred by the file descripto
    
    3.Bind: After creation of the socket, bind function binds the socket to the address and port number specified in addr

    4.Listen: It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection.

    5.Accept: It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.

* **Stages for Client**

    1.Socket connection: Exactly same as that of server’s socket creation
    
    2.Connect: The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr.

By using the mutex lock, the Synchronization is achieved in the  chatroom.

## How to run

1. Clone the project

2. Run the following commands in Linux command terminal/ Windows subsystem for linux:
```bash
  g++ server.cpp -lpthread -o server
  g++ client.cpp -lpthread -o client
```

3. To run the server application, use this command in the terminal :

```bash
  ./server
```

4. Now, open another terminal and use this command to run the client application :
 
```bash
  ./client
```
5. Repeat step-4 to create multiple clients.



## Resources


* [Threads functions](https://www.geeksforgeeks.org/thread-functions-in-c-c/)

* [Mutex lock ](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)

* [Socket Programming](https://www.geeksforgeeks.org/socket-programming-cc/)

*  [Handling multiple clients on server ](https://www.geeksforgeeks.org/handling-multiple-clients-on-server-with-multithreading-using-socket-programming-in-c-cpp/)


## Learning from project

In this project, I have learned how the server handle the multiple clients, the working of the sockets between the server and clients.
 I have learned the multithreading concepts and the synchronization of threads by using mutex. And the POSIX thread functions in c++ of linux system. 