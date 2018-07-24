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

//int main() //CRC - TRANSMISSION
//{
//	// TEST - ARGUMENTS OF THE FUNCTION
//	uint8_t data = 0b10011101;
//	uint8_t dataLength = 8;
//	printf("DATA AS ARGUMENT : %X\n", data);
//	// TEST - ARGUMENTS OF THE FUNCTION
//
//	//Initialize buffer to operate CRC on it
//	buff = data;
//	printf("BUFFER OF DATA : %X\n", buff);
//
//	//Add N zeroes corresponding to the order of the polynomial
//	buff = buff << POLY_ORDER;
//	printf("BUFFER OF DATA WITH ORDER N ZEROS ADDED AT AT LSB : %X\n", buff);
//
//	//Register MSB position of the buffer for a first time
//	int nextMsbPositionBuffer = dataLength + POLY_ORDER;
//
//	while ((buff >> (nextMsbPositionBuffer - 1)) == 0 && nextMsbPositionBuffer != 0) { nextMsbPositionBuffer--; }
//	if (nextMsbPositionBuffer <= 0)
//		printf("ERROR : msbPositionBuffer is less or equal to zero");
//	printf("MSB POSITION IN BUFFER AT START : %d\n", nextMsbPositionBuffer);
//
//	//Calculate remainder for a first time
//	rem = (buff >> (nextMsbPositionBuffer - (POLY_ORDER + 1))) ^ POLY_Gx;
//	printf("CALCULATE FIRST REMAINDER : %X\n", rem);
//
//	//Operaiton loop to find final remainder (XOR bitwise with polynomial(Gx) on buffer)
//	int i;
//	while (nextMsbPositionBuffer > (POLY_ORDER + 1)){//(nextMsbPositionBuffer >= 0){
//		//Verify next MSB position in remainder
//		while (rem != 0 && (rem >> (POLY_ORDER - i)) == 0 && (POLY_ORDER - i) > 0){ i++;}
//
//		//Verify number of bits to add remainder to be able to XOR bitwise again
//		if (rem == 0) {
//			nextMsbPositionBuffer -= (POLY_ORDER + 1);
//			while ((((buff << (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (nextMsbPositionBuffer - 1)) == 0 && nextMsbPositionBuffer != 0) {nextMsbPositionBuffer--;}
//			rem = (((buff << (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (nextMsbPositionBuffer - (POLY_ORDER + 1)));
//			printf("\tREM : %X\n", rem);
//
//			//Calculate next remainder
//			rem = rem ^ POLY_Gx;
//			printf("\tREM : %X\n", rem);
//		}
//		else{
//			if (nextMsbPositionBuffer > (POLY_ORDER + 1)) { //DO SOMETHING WITH THAT
//				nextMsbPositionBuffer -= (POLY_ORDER + 1 - i);
//
//				unsigned long long temp1 = (rem << i);
//				unsigned long long temp21 = (buff << (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer));
//				unsigned long long temp22 = (temp21 >> (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer));
//				unsigned long long temp23 = (temp22 >> nextMsbPositionBuffer);//(temp22 >> (nextMsbPositionBuffer - i)); 
//
//				unsigned long long temp3 = temp1 + temp23;
//				//rem = (rem << i) + (((buff << (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (SIZE_IN_BITS_BUFFER - nextMsbPositionBuffer)) >> (nextMsbPositionBuffer - i));
//				rem = temp3;
//
//				printf("\tREM : %X\n", rem);
//
//				//Calculate next remainder
//				rem = rem ^ POLY_Gx;
//				printf("\tREM : %X\n", rem);
//			}
//		}
//		i = 1;
//
//		////Calculate next remainder
//		//rem = rem ^ POLY_Gx;
//		//printf("\tREM : %X\n", rem);
//	}
//
//	//rem = rem
//
//
//	//Adding CRC value to the buffer
//	buff += rem;
//	
//	// VERIFYING FUNCTION QUALITY
//	printf("REMAINDER EXPECTED: %X\n", 0b100);
//	printf("REMAINDER RESULT: %X\n", rem);
//	printf("BUFFER+CRC EXPECTED: %X\n", 0b10011101100);
//	printf("BUFFER+CRC RESULT: %X\n", buff);
//	if (0b10011101100 == buff && rem == 0b100) 
//		printf("FUNCTION DONE\n", buff);
//	else
//		printf("FUNCTION FAILED\n", buff);
//
//	return 0;
//}

int main() //CRC - TRANSMISSION
{
	// TEST - ARGUMENTS OF THE FUNCTION
	uint8_t data = 0b10011101;
	uint8_t dataLength = 8;
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

	//Calculate remainder for first time
	rem = (buff >> (remBitsToConsiderBuffer - (POLY_ORDER + 1))) ^ POLY_Gx;
	printf("CALCULATE FIRST REMAINDER : %X\n", rem);

	//Initialize variable indicating needed bits to have enough bits to recalculate a XOR bitwise operation
	int neededBitsToRem = 0;

	//Operation loop to find final remainder (XOR bitwise with polynomial(Gx) on buffer)
	while (remBitsToConsiderBuffer > 0){
		//Check where is MSB in remainder
		while ((rem >> (nextMsbPositionRem - 1)) == 0 && nextMsbPositionRem != 0) { nextMsbPositionRem--; }

		//Check bits needed for remainder
		neededBitsToRem = (POLY_ORDER + 1) - nextMsbPositionRem;

		//Fill remainder with needed bits for re-operation XOR bitwise
		rem = rem << neededBitsToRem;
		rem += (buff << (SIZE_IN_BITS_BUFFER - (remBitsToConsiderBuffer - )))
				>>
				(SIZE_IN_BITS_BUFFER - (remBitsToConsiderBuffer - ));

		//Calculate next remainder (XOR bitwise)
		rem = rem ^ POLY_Gx;
		printf("REM : %X\n", rem);
	}

		   	
	// VERIFYING FUNCTION QUALITY
	printf("REMAINDER EXPECTED: %X\n", 0b100);
	printf("REMAINDER RESULT: %X\n", rem);
	if (rem == 0b100) 
		printf("FUNCTION DONE\n", buff);
	else
		printf("FUNCTION FAILED\n", buff);
		   
	return 0;
}