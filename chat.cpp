/*******************************************
* Group Name  : Shannon

* Member1 Name: Shannon
* Member1 SIS ID: 832015157
* Member1 Login ID: shan1020

* Member2 Name: Reece
* Member2 SIS ID: 831066972
* Member2 Login ID: reecedw
********************************************/
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

void checkHostName(int hostname) {
	if (hostname == -1) {
		perror("gethostname() failed\n");
		exit(1);
	}
}

void checkHostEntry(struct hostent* hostentry) {
	if (hostentry == NULL) {
		perror("gethostbyname() failed\n");
		exit(1);
	}
}

void checkSocketStatus(int status) {
	if (status < 0) {
		perror("socket() failed\n");
		exit(1);
	}
}

void checkAddressValidity(int status) {
	if (status <= 0) {
		perror("IP address invalid\n");
		exit(1);
	}
}

void checkBindStatus(int status) {
	if (status < 0) {
		perror("Bind() failed\n");
		exit(1);
	}
}

void checkAcceptStatus(int status) {
	if (status < 0) {
		perror("Server failed to accept incoming connecting\n");
		exit(1);
	}
}

void checkConnectStat (int status) {
	if (status < 0) {
		perror("Client connect() failed\n");
		exit(1);
	}
}

void checkAddressAlreadyinUse (int fd) {
	int yes=1;

	if (setsockopt(fd, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
   		 perror("setsockopt");
   		 exit(1);
	} 	
}

int main(int argc, char* argv[]){

	int server_flag = 0;
	int client_flag = 0;
	char *client_ip;
	int client_port;

	//process command line arguments
	if (argc == 1) {
		server_flag = 1;
	} else if (argc == 2) {
		printf("Make sure to run the client side this way:\n");
		printf("./chat -p [PORT NUMBER] -s [IP ADDRESS]\n");
		printf("Make sure to run the server side this way:\n");
		printf("./chat\n");
	} else {
		client_flag = 1;
		char *port_arg;
		char *ip_arg;
		for (int i = 0; i < argc; i++) {	
			if (strcmp(argv[i], "-p") == 0) {
				port_arg = argv[++i];
				for (int j = 0; j < strlen(port_arg); j++) {
					if (isdigit(port_arg[j])) {
						continue;
					}
					else {
						printf("Port number must be all digits. Please try again.\n");
						exit(0);
					}
				}
				client_port = atoi(port_arg);
			}
			if (strcmp(argv[i], "-s") == 0) {
				ip_arg = argv[++i];
				for (int j = 0; j < strlen(ip_arg); j++) {
					if (isdigit(ip_arg[j]) || ip_arg[j] == '.') {
						continue;
					}
					else {
						printf("IP number must be all digits excluding periods. Please try again.\n");
						exit(0);
					}
				}
				client_ip = ip_arg;
			}
		} 
	}

	if (server_flag == 1) {
		char hostbuffer [256]; 			//holds host name
		char *IPbuffer; 	   			//holds IPv4 address
		struct hostent *host_entry;
		int hostname;

		int socket_status;
		int address_status;
		int bind_status;

		int port = 3493;

		int new_socket;

		char incoming_message[200];
		char outgoing_message[200];

		hostname = gethostname(hostbuffer, sizeof(hostbuffer));
		checkHostName(hostname);

		host_entry = gethostbyname(hostbuffer);
		checkHostEntry(host_entry);

		IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

		socket_status = socket(AF_INET, SOCK_STREAM, 0);
		checkSocketStatus(socket_status);

		struct sockaddr_in sa;
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		memset(&(sa.sin_zero), 0, sizeof(sa.sin_zero));
		address_status = inet_pton(AF_INET, IPbuffer, &(sa.sin_addr));
		checkAddressValidity(address_status);

		bind_status = bind(socket_status, (struct sockaddr *) &sa, sizeof(sa));
		checkAddressAlreadyinUse(socket_status);
		checkBindStatus(bind_status);


		listen(socket_status, 5);
		
		printf("Welcome to Chat!\n");
		printf("Waiting for a connection on %s port %d\n", IPbuffer, port);

		new_socket = accept(socket_status, NULL, NULL);
		checkAcceptStatus(new_socket);

		printf("Connected to a friend! You recieve first.\n");

		while(1) {
			recv(new_socket, incoming_message, sizeof(incoming_message), 0);
			printf("Friend: %s\n", incoming_message);
			printf("You: ");
			fgets(outgoing_message, 141, stdin);
			send(new_socket, outgoing_message, sizeof(outgoing_message), 0);
		}

		close(new_socket);

	}

	if (client_flag == 1) {
		char outgoing_message[200] = "";
		char incoming_message[200] = "";

		int client_socket = socket(AF_INET, SOCK_STREAM, 0);
		checkSocketStatus(client_socket);

		struct sockaddr_in sa;
		sa.sin_family = AF_INET;
		sa.sin_port = htons(client_port);
		memset(&(sa.sin_zero), 0, sizeof(sa.sin_zero));
		int client_addr_stat = inet_pton(AF_INET, client_ip, &sa.sin_addr);
		checkAddressValidity(client_addr_stat);

		int connection_stat = connect(client_socket, (struct sockaddr *)&sa, sizeof(sa));
		printf("Connecting to server...\n");
		checkConnectStat(connection_stat);
		printf("Connected!\n");
		printf("Connected to a friend! You send first.\n");

		while(1){
			printf("You: ");
			fgets(outgoing_message, 140, stdin);
			send(client_socket, outgoing_message, sizeof(outgoing_message), 0);
			recv(client_socket, incoming_message, sizeof(incoming_message), 0);
			printf("Friend: %s\n", incoming_message);
		}

		close(client_socket);



		
	}

	return 0;
}







//You can ignore everything below this. This is Reece's initial work and notes for reference. 
/*
/*******************************************
* Group Name  : XXXXXX

* Member1 Name: Reece Watkins 
* Member1 SIS ID: 831066972
* Member1 Login ID: reecedw

* Member2 Name: XXXXXX
* Member2 SIS ID: XXXXXX
* Member2 Login ID: XXXXXX


#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <winsock.h>

#define PORT 3490

Server 
1. Set up a TCP port and listen for connections (print out IP and PORT is listening on). 
2. Accept connection from client 
3. Block to receive a message from the client. 
4. Receive message and print to screen. 
5. Prompt the user for a message to send. 
6. Send the message to the client. 
7. GOTO step 3. 


//copy and pasted from book at slide 17/page 14
struct addrinfo {
  int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
  int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
  int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
  int ai_protocol; // use 0 for "any"
  size_t ai_addrlen; // size of ai_addr in bytes
  struct sockaddr *ai_addr; // struct sockaddr_in or _in6
  char *ai_canonname; // full canonical hostname
  struct addrinfo *ai_next; // linked list, next node
};


//copy and pasted from book at slide 18/page 15
struct sockaddr_in {
  short int sin_family; // Address family, AF_INET
  unsigned short int sin_port; // Port number
  struct in_addr sin_addr; // Internet address
  unsigned char sin_zero[8]; // Same size as struct sockaddr
};


//Server
int server(){
  //need to set up a TCP port and listen for connections
  //print out IP and PORT is listening on
  int sock, length, fromlen, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[1024];

  //when receiving data, we want to convert from host byte order to network byte order
  //when sending data, we want to convert from network byte order to host byte order

  

}

//Copy and pasted from Book at page 38(in book) or 41st slide
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
Client 
1. Set up a TCP connection to the server on the IP and port specified. 
2. Prompt the user for a message to send. 
3. Send the message to the server. 
4. Block to receive a message from the server. 
5. Receive message and print to screen. 
6. GOTO step 2.
Note: The programs continue until terminated (ctrl

int main(int argc, char* argv[]){

  return 0;
} */