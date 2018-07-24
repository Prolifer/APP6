#include <stdio.h>
#include <stdint.h>

//DEFINITIONS KNOWN AND HARDCODED ON BOTH SERVER/CLIENT SIDE
#define POLY_ORDER	3
#define POLY_Gx		0b1001
#define SIZE_IN_BITS_BUFFER 64 //8*sizeof(long long)/sizeof(uint8_t)

unsigned long long buff = 0; //TODO : Change type long long for a type requiring less digits knowing the max length of the data (for optimization)
unsigned long long rem = 0; //TODO : Change type long long for a type requiring less digits knowing the max length of the data (for optimization)

uint8_t dataArr[200] = ""; //TODO : Experiment with real APP situation
uint8_t dataCRCArr[204] = "";

int main() //CRC - TRANSMISSION
{
	// TEST - ARGUMENTS OF THE FUNCTION
	int data = 0b100111010;
	int dataLength = 9;
	printf("DATA AS ARGUMENT : %X\n", data);
	// TEST - ARGUMENTS OF THE FUNCTION

	//Initialize buffer to operate CRC on it
	buff = data;
	printf("BUFFER OF DATA : %X\n", buff);

	//Add N zeroes corresponding to the order of the polynomial
	buff = buff << POLY_ORDER;
	printf("BUFFER OF DATA WITH ORDER N ZEROS ADDED AT AT LSB : %X\n", buff);

	//Register remaining bits of the buffer for a first time
	int remBitsToConsiderBuffer = dataLength + POLY_ORDER;
	while ((buff >> (remBitsToConsiderBuffer - 1)) == 0 && remBitsToConsiderBuffer != 0) { remBitsToConsiderBuffer--; }
	if (remBitsToConsiderBuffer <= 0)
		printf("ERROR : msbPositionBuffer is less or equal to zero, no bits to consider at start");
	printf("REMAINING BITS TO CONSIDER AT START : %d\n", remBitsToConsiderBuffer);

	//Initialize variable exposing MSB position in remainder
	int nextMsbPositionRem = (POLY_ORDER + 1); //Remainder always has a MSB position at the 4th digit or less

	//Initialize variable indicating needed bits to have enough bits to recalculate a XOR bitwise operation
	int neededBitsToRem = 0;

	//Calculate remainder for first time
	rem = (buff >> (remBitsToConsiderBuffer - (POLY_ORDER + 1)));
	printf("CALCULATE FIRST REMAINDER : %X\n", rem);

	//Operation loop to find final remainder (XOR bitwise with polynomial(Gx) on buffer)
	while (remBitsToConsiderBuffer > 0){
		//Calculate next remainder (XOR bitwise)
		rem = rem ^ POLY_Gx;
		printf("REM : %X\n", rem);

		//Check where is MSB in remainder
		nextMsbPositionRem = (POLY_ORDER + 1); //Remainder always has a MSB position at the 4th digit or less
		while ((rem >> (nextMsbPositionRem - 1)) == 0 && nextMsbPositionRem != 0) { nextMsbPositionRem--; }

		//Check bits needed for remainder
		neededBitsToRem = (POLY_ORDER + 1) - nextMsbPositionRem;

		if ((remBitsToConsiderBuffer - (POLY_ORDER + 1)) >= 0) {
			//Calculate remaining bits to consider in the buffer
			remBitsToConsiderBuffer -= (POLY_ORDER + 1);
			printf("REMAINING BITS CONSIDER BUFFER : %d\n", remBitsToConsiderBuffer);

			//Fill remainder with needed bits for re-operation XOR bitwise
			if(neededBitsToRem != 0){ //IMPORTANT****
			rem = rem << neededBitsToRem;
			rem += (buff << (SIZE_IN_BITS_BUFFER - remBitsToConsiderBuffer))
				>>
				(SIZE_IN_BITS_BUFFER - neededBitsToRem);
			}

			printf("REM : %X\n", rem);
		}
		else {//remBitsToConsiderBuffer < 0
			
			neededBitsToRem = remBitsToConsiderBuffer - 1;
			rem = rem << neededBitsToRem;
			rem += (buff << (SIZE_IN_BITS_BUFFER - neededBitsToRem))
				>>
				(SIZE_IN_BITS_BUFFER - neededBitsToRem);
			remBitsToConsiderBuffer = 0;
			printf("REMAINING BITS CONSIDER BUFFER : %d\n", remBitsToConsiderBuffer);
		}
	}

	// VERIFYING FUNCTION QUALITY
	printf("REMAINDER EXPECTED: %X\n", 0b1000);
	printf("REMAINDER RESULT: %X\n", rem);
	if (rem == 0b1000)
		printf("FUNCTION DONE\n", buff);
	else
		printf("FUNCTION FAILED\n", buff);

	return 0;
}