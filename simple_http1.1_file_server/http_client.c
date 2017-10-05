

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]){
	//char * address;
	//address = argv[1];
	
	
	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//char * portt;
	//portt = argv[2];
	
	//connect to an address
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(8888);
	remote_address.sin_addr.s_addr = INADDR_ANY;  //inet_addr("localhost"); 
	
	//inet_aton(address, &remote_address.sin_addr.s_addr);
	//client_address.sin_addr.s_addr = INADDR_ANY;
	
	
	connect(client_socket, (struct sockaddr * )&remote_address, sizeof(remote_address));
	
	printf("client connected with server...! \n");
	
	char request[] = "GET /index.html HTTP/1.1\r\n Host: www.host.com";
	char response[7000];
	
	send(client_socket, request, sizeof(request), 0);
	
	recv(client_socket, &response, sizeof(response), 0);
	
	printf("Response from the server: %s\n", response);
	
	close(client_socket);
	
	return 0;
	
}
