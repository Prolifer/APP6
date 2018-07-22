/*
Simple UDP Server
Silver Moon ( m00n.silv3r@gmail.com )
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <stdio.h>
#include <winsock2.h>
#include "server.h"
#include "application.h"

#include <iostream>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 25010 //8888   //The port on which to listen for incoming data

int server()
{
	char foo;

	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);

	//***
	//The WSAStartup function is used to start or initialise winsock library.
	//It takes 2 parameters ; the first one is the version we want to load and
	//second one is a WSADATA structure which will hold additional information 
	//	after winsock has been loaded.

	//If any error occurs then the WSAStartup function would return a non zero
	//value and WSAGetLastError can be used to get more information about 
	//what error happened.

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	//***

	//***
	//If a value of 0 is specified, the caller does not wish to specify a
	//protocol and the service provider will choose the protocol to use.
	//When the af parameter is AF_INET or AF_INET6 and the type is SOCK_RAW,
	//the value specified for the protocol is set in the protocol field of 
	//the IPv6 or IPv4 packet header.

	//Create a socket
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_IP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");
	//***

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		printf("buf : %s\n", buf);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		printf("buf : %s\n", buf);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("buf : %s\n", buf);

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buf);
		
		//now reply the client with the same data
		if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		printf("buf : %s\n", buf);

		std::cin >> foo;
	}

	closesocket(s);
	WSACleanup();

	return 0;
}