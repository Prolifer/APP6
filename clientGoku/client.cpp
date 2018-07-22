/*
Simple udp client
Silver Moon (m00n.silv3r@gmail.com)
*/



#include<stdio.h>
#include<winsock2.h>
#include "client.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma warning (disable : 4996)

#define SERVER "127.0.0.1"  //ip address of udp server

#define BUFLEN 512  //Max length of buffer
#define PORT 49178 //25010 //8888   //The port on which to listen for incoming data

int client(void)
{

	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	//start communication
	while (1)
	{
		printf("Message : %s\n",message);

		printf("Enter message : ");
		gets(message);

		printf("Message : %s\n", message);

		//send the message
		if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("Message : %s\n", message);

		////receive a reply and print it
		////clear the buffer by filling null, it might have previously received data
		//memset(buf, '\0', BUFLEN);

		//printf("buf : %s\n", buf);

		////try to receive some data, this is a blocking call
		//if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		//{
		//	printf("recvfrom() failed with error code : %d", WSAGetLastError());
		//	exit(EXIT_FAILURE);
		//}

		//printf("buf : %s\n", buf);

		//puts(buf);

		//printf("buf : %s\n", buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}