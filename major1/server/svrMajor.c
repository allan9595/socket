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
This is server code
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
#include<pthread.h>
#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100
int main() {
    struct sockaddr_in addr, cl_addr;
    int sockfd, len, ret, newsockfd;
    char buffer[BUF_SIZE];
    pid_t childpid;
    char clientAddr[CLADDR_LEN];
    char options;
    char filename[256];
    int bytesReceived = 0;
    char recvBuff[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket!\n");
        exit(1);
    }
    printf("Socket created...\n");
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = PORT;
    ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret < 0) {
        printf("Error binding!\n");
        exit(1);
    }
    printf("Binding done...\n");
    printf("Waiting for a connection...\n");
    listen(sockfd, 5);
    for (;;) { //infinite loop
        len = sizeof(cl_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
        if (newsockfd < 0) {
            printf("Error accepting connection!\n");
            exit(1);
        }
        printf("Connection accepted...\n");
        inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
        
        if ((childpid = fork()) == 0) { //creating a child process
            close(sockfd); 
            //stop listening for new connections by the main process. 
            //the child will continue to listen. 
            //the main process now handles the connected client.
            recvfrom(newsockfd, &options, 1, 0, (struct sockaddr *) &cl_addr, &len);
            if(strcmp(&options,"a")==0){
                for (;;) {
                    memset(buffer, 0, BUF_SIZE);
                    ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
                    if(ret < 0) {
                        printf("Error receiving data!\n");  
                        exit(1);
                    }
                    printf("Received data from %s: %s\n", clientAddr, buffer); 
                    ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);   
                    if (ret < 0) {  
                        printf("Error sending data!\n");  
                        exit(1);  
                    }  
                    printf("Sent data to %s: %s\n", clientAddr, buffer);
                }
            }
            
        }
        if(strcmp(&options,"b")==0){
            read(newsockfd,filename,256);
            FILE *fp;
            fp = fopen(filename, "ab"); 
            if(NULL == fp)
            {
                printf("Error opening file");
                return 1;
            }
            /* Receive data in chunks of 256 bytes */
            while((bytesReceived = read(newsockfd, recvBuff, 256)) > 0)
            {
                printf("Bytes received %d\n",bytesReceived);    
                // recvBuff[n] = 0;
                fwrite(recvBuff, 1,bytesReceived,fp);
                // printf("%s \n", recvBuff);
            }
            if(bytesReceived < 0)
            {
                printf("\n Read Error \n");
            }
        }
        
    }
    
    close(newsockfd);
    return 0;
}
