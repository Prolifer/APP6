#include <stdio.h>
#include <stdint.h>

#define POLY_ORDER	3
#define POLY_Gx		0b1001

long long buff = 0; //TODO : Change type long long for a type requiring less digits knowing the max length of the data (for optimization)
long long rem = 0; //TODO : Change type long long for a type requiring less digits knowing the max length of the data (for optimization)

int main() //CRC - TRANSMISSION
{
	// TEST - ARGUMENTS OF THE FUNCTION
	uint8_t data = 0b10011101;
	uint8_t dataLength = 8;
	printf("%X\n", data);
	// TEST - ARGUMENTS OF THE FUNCTION

	//Initialize buffer to operate CRC on it
	buff = data;
	printf("%X\n", buff);

	//Add N zeroes corresponding to the order of the polynomial
	buff = buff << POLY_ORDER;
	printf("%X\n", buff);

	//Register MSB position of the buffer
	int msbPositionBuffer = dataLength + POLY_ORDER;
	printf("%d\n", msbPositionBuffer);

	while((buff >> (msbPositionBuffer-1)) == 0 ) {msbPositionBuffer--;}
	printf("%d\n", msbPositionBuffer);

	//Operaiton loop
	rem = (buff >> (msbPositionBuffer - (POLY_ORDER + 1))) ^ POLY_Gx;
	//while (1){
	//	//Ensure N digits


	//	printf("%X\n", rem);
	//}
	


	return 0;
}