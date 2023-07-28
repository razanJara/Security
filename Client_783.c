/***************************************************
Name:Razan Monther Solaiman Jaradat.
ID:1931783.
****************************************************/
#include <arpa/inet.h> 
#include <netdb.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h> 
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
//***************************************************
//This function will provide encription and decryption for strings
//***************************************************
char* XORCipher(char* data ) {
	char* key="C5";
	size_t dataLen=strlen(data);
	size_t keyLen=strlen(key);
	char* output = (char*)malloc(sizeof(char) * dataLen);
	for (size_t i = 0; i < dataLen; ++i) {
		output[i] = data[i] ^ key[i % keyLen];
	}
	return output;
}
//***************************************************
//This function will provide encription and decryption for integers
//***************************************************
void xoring(int * array)
{
	for(int i=0;i<3;i++)
	{
		if(i%2==0)
		*array=*array^67;
		else
		*array=*array^53;
		array++;
	}
}
//***************************************************
//This is the client function
//***************************************************
void func(int sockfd,char* type,char* port,char* cORp)
{
//***************************************************
//This part for variable decleration
//****************************************************************
	char buff[MAX];
	int timeT[3];
//***************************************************
//This part is to send if the data encrypted or not
//****************************************************************
		write(sockfd, cORp, sizeof(cORp));
		read(sockfd, buff, sizeof(buff));
//****************************************************************
//This part is for send/recive clear text
//****************************************************************
		if(strncmp("p",cORp,1)==0)
		{
		bzero(buff, sizeof(buff));
		//***************************************************
		//Send to the server what the client whant (date/time)
		//***************************************************
		write(sockfd, type, sizeof(type));
		printf("********************\n");
		read(sockfd,buff,sizeof(buff));
		//***************************************************
		//Send to the server what port the client connect through
		//***************************************************
		write(sockfd, port, sizeof(port));
		bzero(buff, sizeof(buff));
		//***************************************************
		//Recive the respond from the server and print it to the client's terminal
		//***************************************************
		read(sockfd, timeT, sizeof(timeT));
		if(strncmp("time",type,4)==0)
		printf("%s is: %02d:%02d:%02d\n",type, timeT[0],timeT[1],timeT[2]);
		else if(strncmp("date",type,4)==0)
		printf("%s is: %02d\\%02d\\%02d\n",type, timeT[0],timeT[1],timeT[2]);
		}
//****************************************************************
//This part is for send/recive cipher text
//****************************************************************
		else if(strncmp("c",cORp,1)==0)
		{
		bzero(buff, sizeof(buff));
		strcpy(type,XORCipher(type));
		write(sockfd, type, sizeof(type));
		strcpy(type,XORCipher(type));
		printf("********************\n");
		read(sockfd,buff,sizeof(buff));
		strcpy(buff,XORCipher(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, timeT, sizeof(timeT));
		xoring(timeT);
		if(strncmp("time",type,4)==0)
		printf("%s is: %02d:%02d:%02d\n",type, timeT[0],timeT[1],timeT[2]);
		else if(strncmp("date",type,4)==0)
		printf("%s is: %02d\\%02d\\%d\n",type, timeT[0],timeT[1],timeT[2]);
		strcpy(port,XORCipher(port));
		write(sockfd, port, sizeof(port));
		}
//***************************************************			
}
//****************************************************************
//This is the main
//***************************************************
int main(int argc, char **argv)
{
//This part for variable decleration
//****************************************************************
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	cli.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_addr.s_addr = inet_addr((argv[2]));
	servaddr.sin_port = htons(atoi(argv[3]));
//**********************************************************************************
//while client is running...
//**********************************************************************************
	printf("The client IP is: %d.%d.%d.%d  \n",
	(int)(cli.sin_addr.s_addr&0xFF),
	(int)((cli.sin_addr.s_addr&0xFF00)>>8),
	(int)((cli.sin_addr.s_addr&0xFF0000)>>16),
	(int)((cli.sin_addr.s_addr&0xFF000000)>>24));
	//printf("port is: %d\n", (int) ntohs(servaddr.sin_port));
//**********************************************************************************	
// connect the client socket to server socket
//**********************************************************************************
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0)
		exit(0);
		char portNo[5];
		read(sockfd,portNo, sizeof(portNo));
		printf("Client port is: %s\n",portNo);
//**********************************************************************************
//Call The fction to start sending/reciving packets to/from the server...
//**********************************************************************************
	func(sockfd,argv[4],argv[3],argv[1]);
//**********************************************************************************
//Finish and close the socket
	//close(sockfd);
//**********************************************************************************
}
