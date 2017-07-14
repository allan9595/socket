/*AUTHORS: 

BOHAN ZHANG
JESSICA LOWE
Jaden Staats

Date:5/9/2017
CLASS CSCE 3600.001

This programn will run three parts when you get into the menu. At the menu 
stage, please enther in 'a', 'b' or 'c' to choose whichever the options you 
want. In option a, the user can get into a message system and having chat with 
ther server. At the optioon b, the client should be able to copy fiel to 
server side. Option C is a quit option. You should type in your server IP address
after you ./a.out. And the program maybe imperfect in some level and stage.

Purpose: 
This is client code
message chatting
file copy 
quit
*/


#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"
#define PORT 4444
#define BUF_SIZE 2000
int main(int argc, char**argv) {
    struct sockaddr_in addr, cl_addr;  
    int sockfd, ret;  
    char buffer[BUF_SIZE];  
    struct hostent * server;
    char * serverAddr;
    char filename[256];
    if (argc < 2) {
        printf("usage: client < ip address >\n");
        exit(1);  
    }
    serverAddr = argv[1]; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd < 0) {  
        printf("Error creating socket!\n");  
        exit(1);  
    }  
    printf("Socket created...\n");   
    memset(&addr, 0, sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(serverAddr);
    addr.sin_port = PORT;     
    ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
    if (ret < 0) {  
        printf("Error connecting to the server!\n");  
        exit(1);  
    }  
    printf("Connected to the server...\n");  
    memset(buffer, 0, BUF_SIZE);
    while(1){
        char options;
        printf("Type in your options: a.message b. putfile q. quit\n");
        scanf("%c",&options);
        sendto(sockfd, &options,1, 0, (struct sockaddr *) &addr, sizeof(addr));
        do{
            switch(options){
            case 'a':
                printf("Enter your message(s): \n");
                while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
                    ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr)); 
                    if (ret < 0) {
                        printf("Error sending data!\n\t-%s", buffer);
                    }
                    ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
                    if (ret < 0) {
                        printf("Error receiving data!\n");
                    } else {
                        printf("Received: ");
                        fputs(buffer, stdout);
                        printf("\n");
                    }
                }
            break;
            case 'b':
                printf("Filename: \n");
                scanf("%s",filename);
                write(sockfd,filename,256);
                FILE *fp = fopen(filename,"rb");
                if(fp==NULL){
                    perror("File failed");
                    return 1;
                }
                while(1)
                {
                    
                    /* First read file in chunks of 256 bytes */
                    unsigned char buff[256]={0};
                    int nread = fread(buff,1,256,fp);
                    printf("Bytes read %d \n", nread);        
                    /* If read was success, send data. */
                    if(nread > 0)
                    {
                        printf("Sending \n");
                        write(sockfd, buff, nread);
                    }
                    
                    if (nread < 256)
                    {
                        if (feof(fp))
                        printf("End of file\n");
                        if (ferror(fp))
                        printf("Error reading\n");
                        break;
                    }
                }
            break;
			case 'q':
				break;
            default:
               break;
            
            }
            close(sockfd);
            
        
        }while(options!='q');
    }
    return 0;
}
