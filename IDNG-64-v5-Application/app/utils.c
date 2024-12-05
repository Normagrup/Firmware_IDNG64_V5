#include "utils.h"
#include "macros.h"



        
// Devuelve 0x01 si el valor el bit está marcado en el array de 64 bits y 0x00 en caso contrario.
unsigned char utils_BitFromByteArrayGet (unsigned char *byteArray, unsigned char device)
{
	unsigned char byte;
	unsigned char bit;
	unsigned char answer;
	byte=device/8;
	bit=device-(8*byte);
	answer=BITVAL(byteArray[byte],bit);
	return answer;
}


void 	utils_BitFromByteArraySet (unsigned char *byteArray, unsigned char device)
{
	unsigned char byte;
	unsigned char bit;		
	byte=device/8;
	bit=device-(8*byte);	
	SETBIT(byteArray[byte],bit);	
}

void utils_BitFromByteArrayClear (unsigned char *byteArray, unsigned char device)
{
	unsigned char byte;
	unsigned char bit;		
	byte=device/8;
	bit=device-(8*byte);	
	CLEARBIT(byteArray[byte],bit);	
}










void bytePush(unsigned char byteIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr)
{
	arrayOut[0]=byteIn;
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+1;
}

void uint16Push(unsigned int uint16In, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr)
{
	arrayOut[0]=(unsigned char)(uint16In/0x0100);
	arrayOut[1]=(unsigned char)(uint16In&0x00FF);
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+2;
}

void arrayPush (unsigned char *arrayIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr, unsigned short int arrayInSize)
{	
	unsigned short int n;	
	for (n=0;n<arrayInSize;n++)
	{
		arrayOut[n]=arrayIn[n];		
	}		
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+arrayInSize;
}



void bytePop(unsigned char *arrayIn, unsigned char *byteOut, unsigned short int *arrayOutOffsetPtr)
{
	*byteOut=arrayIn[0];
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+1;
}

void uint16Pop(unsigned char *arrayIn, unsigned short int *uint16Out, unsigned short int *arrayOutOffsetPtr)
{	
	*uint16Out=(arrayIn[0]*0x0100)+(arrayIn[1]);
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+2;
}

void uint32Pop(unsigned char *arrayIn, unsigned int *uint32Out, unsigned short int *arrayOutOffsetPtr)
{
	*uint32Out=(arrayIn[0]*0x1000000)+(arrayIn[1]*0x010000)+(arrayIn[2]*0x0100)+(arrayIn[3]);
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+2;
}

void arrayPop (unsigned char *arrayIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr, unsigned short int arrayOutSize)
{	
	unsigned short int n;	
	for (n=0;n<arrayOutSize;n++)
	{
		arrayOut[n]=arrayIn[n];		
	}	
	*arrayOutOffsetPtr=(*arrayOutOffsetPtr)+arrayOutSize;
}








	



