/*
 * TCP socket implementation
 * Basic implementation
 *
 * Author- Aman Kanwar
 * Version V1.0
 */

#include<stdio.h>
#include<string.h>
#include<strings.h>    // for the bzero
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define BACKLOG 5

//--------This block will be used to display the error ----------
void error(char *message)
{
    perror(message);
    exit(1);
}
//---------------------------------------------------------------

int main(int argc, char *argv[])
{
   
   //----- Checking for the presence of PORT NUMBER  -----
   
   if(argc < 2){fprintf(stderr,"No Port Entered");exit(1);}
   //   our code will terminate, if no Port Number is provided
   // by the user for the given server to listen to
   // ----------------------------------------------------

   int socketFD, socketFD_new, portNumber, clientLength, readWrite;
   struct sockaddr_in serverAddr,clientAddr;
   char tempBuffer[100];
   
//==================   Creating Socket File Descriptor ================================

   if((socketFD = socket(PF_INET, SOCK_STREAM,0)) < 0)
   {
      fprintf(stderr,"Error encountered in opening socket");
   }
/* Here we have choosen the Protocol value as
    zero, 
    Create a new socket of type TYPE in domain DOMAIN,
    using protocol PROTOCOL. If PROTOCOL is zero, 
    one is chosen automatically. 
    Returns a file descriptor for the new socket, or -1 for errors.
   */
//-------------------------------------------------------------------------
   

// this will write 0s to all the locations of tempBuffer

// -----Now we will collect the Port Number Provided by the user -----
   /* know that the user has entered the Port Number on the terminal, i.e in String Format
    hence, we will use the atoi() function to convert the entered number (string format)
    to the integer format and we will assign the same to our portNumber variable */

    portNumber = atoi(argv[1]);  // Port Number assigned to the given variable 

// ================     BINDING  =============================================    

/*----------------------------------------------------------------------------------------
 An IP socket address is defined as a combination  of  an  IP  interface
       address  and a 16-bit port number.  The basic IP protocol does not sup‐
       ply port numbers, they are implemented by higher level  protocols  like
       udp(7) and tcp(7).  

 > Now we have the Port Number value, we have the Socket File Descriptor of the type
  TCP and a Protocol choosen automatically
 > Now, we need to bind the Port Number with the IP address  

 > Definition of the sturct sockaddr_in and its initilization is provided below: 

 struct sockaddr_in {
               sa_family_t    sin_family; // address family: AF_INET 
               in_port_t      sin_port;   // port in network byte order 
               struct in_addr sin_addr;   // internet address 
           };

           // Internet address.
           struct in_addr {
               uint32_t       s_addr;     // address in network byte order
           }

Note that:-
       sin_family  is  always  set to AF_INET.  
       
       This is required; in Linux 2.2
       most networking functions return EINVAL when this setting  is  missing.
       sin_port  contains  the  port  in network byte order.  The port numbers
       below 1024 are called privileged ports (or sometimes: reserved  ports).
       Only   a   privileged  process  (on  Linux:  a  process  that  has  the
       CAP_NET_BIND_SERVICE capability in the  user  namespace  governing  its
       network  namespace)  may  'bind'  to these sockets.
// -----------------------------------------------------------------------*/

serverAddr.sin_family      = AF_INET;     // for IPV4 
serverAddr.sin_addr.s_addr = INADDR_ANY;  // address in network byte order
     /* INADDR_ANY can have the communication from any link ie wired,wireless, localhost 
        hence, the client need not to look for a connection from a perticular medium
        as the client can now have the connection with the server from any of the given
        medium, LAN, WLAN, localhost */  
serverAddr.sin_port        = htons(portNumber); 
    // this will convert the network byte from small endian (if present) to big endian

if((bind(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)) {error("Bind Error");}

// binding done

//================ Listeing =============================

listen(socketFD,BACKLOG);          //starts listeing for a request
clientLength = sizeof(clientAddr);
// this will gather the length of the client structur

//================ Accept ===========================================================

/*
  Know that, we will create a new socket, once the connection is accepted by the server
  Once, the connection is accepted, it will go into the blocking state

  accept() system call will gather all the data from the client, data like clientAddress,
  IP and PORT number
*/

if((socketFD_new = accept(socketFD, (struct sockaddr *)&clientAddr, &clientLength)) < 0)
{
    error("\nError Accetpting the connection\n");
    exit(1);
}



	char *success = "Received <Sever>";

while(1)
{

/*---------------- Working of the Server Here ----------------------------

*Server will listen and wait for a given message to arrive, once received
 =>server will acknowledge the received message.

*After reception of the message, server will send a message to the client
 =>server will wait for the reception.  (acknowledgement from the client)
--------------------------------------------------------------------------*/


	// --------- Now the connection has been established properly -----------------------
	   // now we can send and receive the data, ie read and write the data
	// Once the socket is created, binded, connected, we need to clear the buffer   
	 bzero(tempBuffer,sizeof(tempBuffer));



	//================   Read  ==========================

	/* ------------------------------------------------------------------------
	definition of read() is provided below: 

	ssize_t read(int fd, void *buf, size_t count);

	read()  attempts to read up to count bytes from file descriptor fd into
	       the buffer starting at buf.

	On files that support seeking, the read operation commences at the file
	       offset, and the file offset is incremented by the number of bytes read.
	       If the file offset is at or past the end of file, no  bytes  are  read,
	       and read() returns zero.

	       If count is zero, read() may detect the errors described below.  In the
	       absence of any errors, or if read() does not check for errors, a read()
	       with a count of 0 returns zero and has no other effects.
	*/

 
       //--------------------- Reception of first message and acknowledgement--------------------------------------
	if((readWrite = read(socketFD_new,tempBuffer,sizeof(tempBuffer))) < 0 )
	{error("Error Reading from Socket");}
        
        if((write(socketFD_new,success,strlen(success))) < 0)
	{error("Error in writing");}

	// once the bytes are read sucessfully, we can print the message
	printf("\nReceived Message is %s\t\t\t====> ACK Sent to Client <===\n",tempBuffer); 
		// we can also, write success on the message receial	
	//--------------------------------------------------------------------------
 
        bzero(tempBuffer,sizeof(tempBuffer));

        //--------------------- Writing to client--------------------------------------
        
        printf("\n_msg_to_client_: ");	
        
        fgets(tempBuffer,sizeof(tempBuffer),stdin);  // take input from the stdinput
         
        if((write(socketFD_new,tempBuffer,strlen(tempBuffer))) < 0)
	{error("Error in writing");}

        bzero(tempBuffer,strlen(tempBuffer));  // clear the buffer again

           // reading for acknowledgement from the client
        if((readWrite = read(socketFD_new,tempBuffer,sizeof(tempBuffer))) < 0 )
	{error("Error Reading from Socket");}
        
	// printing the received acknowledgement from the client
	printf("\t\t\tAcknowledgement: %s\n\n",tempBuffer); 
		// we can also, write success on the message receial

	//--------------------------------------------------------------------------
 }

return 0;
}
