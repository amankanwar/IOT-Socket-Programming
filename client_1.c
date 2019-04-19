/***************************************************
* Here in this code, I have written a basic Client
* socket, which establish connection with the server
* at a given IP address (in our case we have taken)
* localhost and the port on which server is serving
* the requests
* Just Execute the command 
* $-> ./object_file _IP_Address_ _port_number
* ex -> ./client localhost 5000
*
* Author - Aman Kanwar
* Version v1.0
***************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<netdb.h>

void error(char *message)
{
    perror(message);
    exit(0);
}

int main(int argc, char *argv[])
{
    int socketFD, portNumber, n;
    struct sockaddr_in serverAddress;
    struct hostent *server;
    char tempBuffer[1024];

    if(argc<3){fprintf(stderr,"Usage %s hostname port\n",argv[0]);}
     
    portNumber = atoi(argv[2]); 

    if((socketFD = socket(AF_INET,SOCK_STREAM, 0)) < 0){error("Socket issue");}
     
    server = gethostbyname(argv[1]);

    if(server == NULL)
    {
        fprintf(stderr,"No Such Host");
    } 
    // here we will get the IP address from the parameter passed by the user
    
    bzero((char *)&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;  // address family
    
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);   

    // second way is to use the inet_pton
    // IP has to be in binary format
     /*int inet_pton(int __af, const char *__restrict__ __cp, void *__restrict__ __buf)
       Convert from presentation format of an Internet number in buffer starting at CP 
       to the binary network format and 
       store result for interface type AF in buffer starting at BUF*/

      //inet_pton(serverAddress.sin_family,argv[1],)
    
    serverAddress.sin_port = htons(portNumber);

    if((connect(socketFD, (struct sockaddr*)&serverAddress,sizeof(serverAddress))) < 0)
    {error("\nUnable to establish connection\n");}
    
    // if block comes here then the coonection is established
    
    bzero(tempBuffer,sizeof(tempBuffer));
    printf("\nMessage: ");
    fgets(tempBuffer,1024,stdin);   // taking the input from std input to the tempBuffer

    if((write(socketFD, tempBuffer, strlen(tempBuffer))) < 0){error("\nError writing\n");}

    bzero(tempBuffer,strlen(tempBuffer));  // clear the buffer again

    //reading data from the server
    if((read(socketFD,tempBuffer,sizeof(tempBuffer))) < 0){error("\nError Reading\n");}

    printf("\n_msg_: %s\n",tempBuffer);
    
    return 0;
}