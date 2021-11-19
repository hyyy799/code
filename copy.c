#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET int
#define MAX_REQUEST_SIZE 2047

#define ISVALIDSOCKET(s) ((s) >= 0)

struct client_info {
    	socklen_t address_length;
    	struct sockaddr_storage address;
    	SOCKET socket;
    	char request[MAX_REQUEST_SIZE + 1];
    	int received;
    	struct client_info *next;
};


SOCKET create_socket(const char* host, const char *port);
struct client_info *get_client(SOCKET s);
const char *get_client_address(struct client_info *ci);


int main(){

	SOCKET server = create_socket(0,"8080");
/*
	while(1) {

		//FD_ISSET(fd， &set); 測試fd是否在set集合中

 
            		struct client_info *client = get_client(-1);

            		client->socket = accept(server, (struct sockaddr*) &(client->address), &(client->address_length));

            		if (!ISVALIDSOCKET(client->socket)) {
                		fprintf(stderr, "accept() failed. (%d)\n", errno);
				return 1;
            		}

            		printf("New connection from %s.\n",
                    	get_client_address(client));
        	


        	struct client_info *client = clients;

        	while(client) {
            		struct client_info *next = client->next;

            		

                		if (MAX_REQUEST_SIZE == client->received) {
                    			send_400(client);
                    			client = next;
                    			continue;
                		}

                		int r = recv(client->socket, client->request + client->received, MAX_REQUEST_SIZE - client->received, 0);

                		if (r < 1) {
                    			printf("Unexpected disconnect from %s.\n", get_client_address(client));
                    			drop_client(client);
	
        	       		} 
				else {
                    			client->received += r;
                    			client->request[client->received] = 0;

    	                		char *q = strstr(client->request, "\r\n\r\n");
                    		
					if (q) {
        	                		*q = 0;
	
                	        		if (strncmp("GET /", client->request, 5)) {
                        	    			send_400(client);
                        			} 
						else {
   	                         			char *path = client->request + 4;
        	                    			char *end_path = strstr(path, " ");
                	            			if (!end_path) {
                        	        			send_400(client);
                            				} 
							else {
                                				*end_path = 0;
                                				serve_resource(client, path);
	                            			}
        	                		}
                	    		} //if (q)
	
        	        	}
            		

            	client = next;
        	
		}

    	} //while(1)


	printf("\nClosing socket...\n");
	CLOSESOCKET(server);
*/
	return 0;

}


SOCKET create_socket(const char* host, const char *port) {
	printf("Configuring local address...\n");
    
	struct addrinfo hints;
    	memset(&hints, 0, sizeof(hints));
   	hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = AI_PASSIVE;

    	struct addrinfo *bind_address;
    	getaddrinfo(host, port, &hints, &bind_address);

    	printf("Creating socket...\n");
    	SOCKET socket_listen;
    	socket_listen = socket(bind_address->ai_family,
        	bind_address->ai_socktype, bind_address->ai_protocol);
    	
	if (!ISVALIDSOCKET(socket_listen)) {
        	fprintf(stderr, "socket() failed. (%d)\n", errno);
        	exit(1);
    	}

    	printf("Binding socket to local address...\n");
    	if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        	fprintf(stderr, "bind() failed. (%d)\n", errno);
        	exit(1);
    	}
   	
	freeaddrinfo(bind_address);

    	printf("Listening...\n");
    	if (listen(socket_listen, 10) < 0) {
        	fprintf(stderr, "listen() failed. (%d)\n", errno);
        	exit(1);
    	}

    	return socket_listen;
}

struct client_info *get_client(SOCKET s) {
	    struct client_info *ci = clients;

    	while(ci) {
        	if (ci->socket == s)
            		break;
        	ci = ci->next;
    	}

    	if (ci) return ci;
    	struct client_info *n = (struct client_info*) calloc(1, sizeof(struct client_info));

    	if (!n) {
        	fprintf(stderr, "Out of memory.\n");
        	exit(1);
    	}

    	n->address_length = sizeof(n->address);
    	n->next = clients;
    	clients = n;
    	return n;
}

const char *get_client_address(struct client_info *ci) {
    	static char address_buffer[100];
    	getnameinfo((struct sockaddr*)&ci->address, ci->address_length, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    	return address_buffer;
}
