
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int response_checker(char http_req[], char dir_from_cmd[]);

int main(int argc, char *argv[]) {
    
    char * port;
    port = argv[1];      //port 1

    char * dir_from_cmd;
    dir_from_cmd = argv[2]; //directory 2  //  /home/mohammad/Downloads

    //create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM,0);         
    
	
    //define the address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(port));
    server_address.sin_addr.s_addr = INADDR_ANY;   //inet_addr("localhost"); 
	
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));  
    
    listen(server_socket, 5);
    int client_socket;
	
    char req_from_client[100];
    while(1){
		
		//server running
		printf("server is running...\n");
		
        	client_socket = accept(server_socket, NULL, NULL);
		//try to get requent string from client
		recv(client_socket, &req_from_client, sizeof(req_from_client), 0);
		printf("client request catched!!!\n");
		//printf("di_from_cmd: %s \n", dir_from_cmd);

		

		//count the length of the dir in string
    		int dir_len =0;
    		if(req_from_client[4]=='/'){
	             int i;for(i=4;i<50;i++){
            		 if(req_from_client[i]!=' '){
                		dir_len++;
            		 }else{
                		break;
            		 }
        	     }
                }
		
		//rebuild the dir
		char dir_from_client[50] ="";
		int i;for(i=4; i<dir_len+4 ;i++){
		    char x = req_from_client[i];
		    int len = strlen(dir_from_client);
		    dir_from_client[len++] = x;
		}
		char final_dir[100]="";
		strcat(final_dir,dir_from_cmd);
		strcat(final_dir, dir_from_client);
		printf("direcotory to get file from: %s \n", final_dir);

		int response_code = response_checker(req_from_client, final_dir);
		if(response_code == 200){
	             //fetch the file 
		     FILE * fPointer;
    		     fPointer = fopen(final_dir, "r");   //opens requested file
		     char data_buf[3000];
 		     while(!feof(fPointer)){
		     fgets(data_buf, 3000, fPointer);
		     }		
		
    		    char response_buf[5000] = "HTTP/1.1 200 OK\r\n\n";
    		    strcat(response_buf, data_buf);

		    //send the response_buf back
        	    send(client_socket, response_buf, sizeof(response_buf), 0);
        	    printf("server sent response: %s \n", response_buf);
        	    close(client_socket);

		    //clean the buffers
		   
		}else if(response_code ==405){
		    char res_buff_405[50]="HTTP/1.1 405 Method Not Allowed";
		    send(client_socket, res_buff_405, sizeof(res_buff_405), 0);
		    close(client_socket);
		}else if(response_code ==404){
		    char res_buff_404[50]="HTTP/1.1 404 File Not Found";
		    send(client_socket, res_buff_404, sizeof(res_buff_404), 0);
		    close(client_socket);
		}else if(response_code == 400){
		    char res_buff_400[100]="HTTP/1.1 400 Bad Request. Try again with corrct HTTP/1.1 format!";
		    send(client_socket, res_buff_400, sizeof(res_buff_400), 0);
		    close(client_socket);
		}
		
	
     }
      
    return 0;
}



int response_checker(char http_req[], char dir_from_cmd[]){
    
    //count the length of the dir in string
    int dir_len =0;
    if(http_req[4]=='/'){
        
        int i;for(i=4;i<100;i++){
            if(http_req[i]!=' '){
                dir_len++;
            }else{
                break;
            }
        }
    }
    
    //check for error code 405 = method not allowed PUT, POST, DELETE
    if((http_req[0]=='P' && http_req[1]=='U' && http_req[2]=='T') || 
       (http_req[0]=='P' && http_req[1]=='O' && http_req[2]=='S' && http_req[3]=='T') || 
       (http_req[0]=='H' && http_req[1]=='E' && http_req[2]=='A' && http_req[3]=='D') || 
       (http_req[0]=='D' && http_req[1]=='E' && http_req[2]=='L' && http_req[3]=='E' && http_req[4]=='T' && http_req[5]=='E') ||
       (http_req[3]!=' ')){
        //printf("method not allowed, error 405;");           //delete
        return 405;                           //uncomment
    }
    
   //check for error code 404 = file not found
    FILE *f;
    f = fopen(dir_from_cmd, "r");
    if(!f){
        //printf("file doesnt exist, error 404\n");  //delete
        return 404;   //uncomment
    }      
    
    //check for error code 200 = OK, 400 = BAD REQ
    if(http_req[0]=='G' && http_req[1]=='E' && http_req[2]=='T' && 
       http_req[4]=='/' && http_req[dir_len+4+1]=='H' &&
       http_req[dir_len+4+2]=='T' && http_req[dir_len+4+3]=='T' &&
       http_req[dir_len+4+4]=='P' && http_req[dir_len+4+5]=='/' &&
       http_req[dir_len+4+6]=='1' && http_req[dir_len+4+8]=='1'){
            //printf("Perfect GET req! 200\n");  //delete it
            return 200;    //uncomment
    }else{
        //printf("Bad Request 400! Check for formatting for HTTP GET req! \n");  //delete it
            return 400;    
    }
    
    return 0;
}



 
 
