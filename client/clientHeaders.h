#ifndef CLIENT_HEADERS
#define CLIENT_HEADERS

#include <stdint.h>

#define PACKET_BITS_LENGTH 64
#define DATA_BITS_PER_PACKET_LENGTH 40
#define HEADERS_BITS_PER_PACKET_LENGTH 24

double buffer = 0;

void packetMaker(char message[], int sizeOfMessageInBits);
void clientTransportHeader();
void clientInternetHeader();
void clientNetworkHeader();

#endif //CLIENT_HEADERS
