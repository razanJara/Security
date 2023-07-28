/***************************************************
Name:Razan Monther Solaiman Jaradat.
ID:1931783.
****************************************************/
#include <stdio.h>
#include<math.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <arpa/inet.h>
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
//This is the server function
//***************************************************
void func(int connfd,struct sockaddr_in cli)
{
//***************************************************
//This part for variable decleration
//****************************************************************
	char buff[MAX];
	char port[5];
	char *flag;
	char*buffer;
	int n;
	int timeT[3];
	int timeD[3];
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	timeT[0]=local->tm_hour;
	timeT[1]=local->tm_min;
	timeT[2]=local->tm_sec;
	timeD[0] = local->tm_mday;            
        timeD[1] = local->tm_mon + 1;      
        timeD[2] = local->tm_year + 1900;
//***************************************************
//This part is to know if the data encrypted or not
//****************************************************************
		read(connfd, buff, sizeof(buff));
		write(connfd,buff,sizeof(buff));
//****************************************************************
//This part is for send/recive clear text
//****************************************************************
		if(strncmp("p",buff,1)==0)
		{
		bzero(buff, sizeof(buff));
		//Read the if the type of req is date or time
		read(connfd, buff, sizeof(buff));
		//***************************************************
		//Print info to the server's terminal
		//***************************************************
		printf("********************\n");
		printf("Request type is %s \n",buff);
		//***************************************************
		//Print info to the server's terminal
		//***************************************************
		printf("Sinding %s to %d.%d.%d.%d  \n",buff,
		(int)(cli.sin_addr.s_addr&0xFF),
		(int)((cli.sin_addr.s_addr&0xFF00)>>8),
		(int)((cli.sin_addr.s_addr&0xFF0000)>>16),
		(int)((cli.sin_addr.s_addr&0xFF000000)>>24));
		write(connfd,buff,sizeof(buff));
		//Read the port number that we communication through
		read(connfd, port, sizeof(port));
		//***************************************************
		//Print info to the server's terminal
		//***************************************************
		printf("Request from %d.%d.%d.%d ",(int)(cli.sin_addr.s_addr&0xFF),
		(int)((cli.sin_addr.s_addr&0xFF00)>>8),
		(int)((cli.sin_addr.s_addr&0xFF0000)>>16),
		(int)((cli.sin_addr.s_addr&0xFF000000)>>24));
		printf(" and port: %s \n",port);
		//***************************************************
		//Send the time or date upon the req
		//***************************************************
		if(strncmp("time",buff,4)==0)
		{
			bzero(buff, sizeof(buff));
			write(connfd,timeT,sizeof(timeT));
			bzero(buff, sizeof(buff));
			
		}
		else if(strncmp("date",buff,4)==0)
		{
		        bzero(buff, sizeof(buff));
			write(connfd,timeD,sizeof(timeD));
			bzero(buff, sizeof(buff));
		}
		bzero(buff, sizeof(buff));
		}
//****************************************************************
//This part is for send/recive cipher text
//****************************************************************
		else if(strncmp("c",buff,1)==0)
		{
		bzero(buff, sizeof(buff));
		//Read the if the type of req is date or time
		read(connfd, buff, sizeof(buff));
		strcpy(buff,XORCipher(buff));
		buff[4]='\0';
		//***************************************************
		//Print info to the server's terminal
		//***************************************************
		printf("********************\n");
		printf("Request type is %s \n",buff);
		//***************************************************
		//Print info to the server's terminal
		//***************************************************
		printf("Sinding %s to %d.%d.%d.%d  \n",buff,
		(int)(cli.sin_addr.s_addr&0xFF),
		(int)((cli.sin_addr.s_addr&0xFF00)>>8),
		(int)((cli.sin_addr.s_addr&0xFF0000)>>16),
		(int)((cli.sin_addr.s_addr&0xFF000000)>>24));
		//***************************************************
		strcpy(buff,XORCipher(buff));
		write(connfd,buff,sizeof(buff));
		strcpy(buff,XORCipher(buff));
		buff[4]='\0';
		//***************************************************
		//Send the time or date upon the req
		//***************************************************
		if(strncmp("time",buff,4)==0)
		{
			bzero(buff, sizeof(buff));
			xoring(timeT);
			write(connfd,timeT,sizeof(timeT));
			bzero(buff, sizeof(buff));
			
		}
		else if(strncmp("date",buff,4)==0)
		{
		        bzero(buff, sizeof(buff));
			xoring(timeD);
			write(connfd,timeD,sizeof(timeD));
			bzero(buff, sizeof(buff));
		}
		bzero(buff, sizeof(buff));
		//***************************************************
		//Read the port number that we communication through
		//***************************************************
		read(connfd, port, sizeof(port));
		strcpy(port,XORCipher(port));
		port[4]='\0';
		//*********************************************************
		//Print info to the server terminal
		//*********************************************************
		printf("Request from %d.%d.%d.%d ",(int)(cli.sin_addr.s_addr&0xFF),
		(int)((cli.sin_addr.s_addr&0xFF00)>>8),
		(int)((cli.sin_addr.s_addr&0xFF0000)>>16),
		(int)((cli.sin_addr.s_addr&0xFF000000)>>24));
		printf(" and port: %s \n",port);
		}
//****************************************************************
	
}
//***************************************************
//This is the main
//***************************************************
int main(int argc, char **argv)
{
//This part for variable decleration
//****************************************************************
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	len = sizeof(cli);
//**********************************************************************************
//while server is running...
//**********************************************************************************
	printf("The server IP is: %d.%d.%d.%d  \n",
	(int)(servaddr.sin_addr.s_addr&0xFF),
	(int)((servaddr.sin_addr.s_addr&0xFF00)>>8),
	(int)((servaddr.sin_addr.s_addr&0xFF0000)>>16),
	(int)((servaddr.sin_addr.s_addr&0xFF000000)>>24));
	printf("port is: %d\n", (int) ntohs(servaddr.sin_port));
//**********************************************************************************
//Binding, Listining & connecting the client...
//**********************************************************************************
	//Binding...
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
		exit(0);
	// Now server is ready to listen...
	if ((listen(sockfd, 5)) != 0) 
		exit(0);
	// Accept the data packet from client...
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) 
		exit(0);
char client_ip[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &cli.sin_addr, client_ip, INET_ADDRSTRLEN);
int client_port = ntohs(cli.sin_port);
char p[5];
sprintf(p,"%d",client_port);
write(connfd,p,sizeof(p));
//**********************************************************************************
//Call The fction to start sending/reciving packets to/from the client...
//**********************************************************************************
	func(connfd,cli);
//**********************************************************************************
//Finish and close the socket
	close(sockfd);
//**********************************************************************************
}
