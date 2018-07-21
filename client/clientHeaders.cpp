#include "clientHeaders.h"

void packetMaker(char message[], int sizeOfMessageInBits){
	int nbUnusedBits = 0;
	int nbPacketsToDo = 0;

	if ((sizeOfMessageInBits % DATA_BITS_PER_PACKET_LENGTH) != 0)
		nbPacketsToDo = sizeOfMessageInBits / DATA_BITS_PER_PACKET_LENGTH + 1;
	else
		nbPacketsToDo = sizeOfMessageInBits / DATA_BITS_PER_PACKET_LENGTH;

	for (int i = 0; i < nbPacketsToDo; i++) {
		for (int y = 0; y < sizeOfMessageInBits; y++) {
			buffer = message[i];

			clientTransportHeader();
			clientInternetHeader();
			clientNetworkHeader();
		}
	}
}

void clientTransportHeader(){

}

void clientInternetHeader(){

}

void clientNetworkHeader(){

}