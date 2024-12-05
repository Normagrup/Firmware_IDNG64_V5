#define STORAGE_OP_STOP	0x00
#define STORAGE_OP_READING 0x01
#define STORAGE_OP_WRITING 0x02



unsigned char storageReadStart (void);
unsigned char storageWriteStart (void);
void storageReadWriteFinish (void);
unsigned int storageIdGet (void);
unsigned char storageWrite (unsigned char *buffer, unsigned int size);
unsigned char storageRead (unsigned char *buffer, unsigned int size);
void storageDoTask (void);
