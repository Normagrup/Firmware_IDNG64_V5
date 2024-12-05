void bytePush(unsigned char byteIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr);
void uint16Push(unsigned int uint16In, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr);
void arrayPush (unsigned char *arrayIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr, unsigned short int arrayInSize);
void bytePop(unsigned char *arrayIn, unsigned char *byteOut, unsigned short int *arrayOutOffsetPtr);
void uint16Pop(unsigned char *arrayIn, unsigned short int *uint16Out, unsigned short int *arrayOutOffsetPtr);
void arrayPop (unsigned char *arrayIn, unsigned char *arrayOut, unsigned short int *arrayOutOffsetPtr, unsigned short int arrayOutSize);
void uint32Pop(unsigned char *arrayIn, unsigned int *uint32Out, unsigned short int *arrayOutOffsetPtr);