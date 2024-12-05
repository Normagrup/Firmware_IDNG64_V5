#define DALI_FRAME_TYPE_SINGLE_SEND                 1
#define DALI_FRAME_TYPE_BATCH_SEND                  0
#define DALI_FRAME_TYPE_SINGLE_RECEIVE							3
#define DALI_FRAME_TYPE_BATCH_RECEIVE								2



#define DALI_BUFFERS_CNT														8
#define DALI_SENSORS																0
#define DALI_NORMALINK															1
#define DALI_WEBSERVER															2
#define DALI_TEST																		3
#define DALI_TEMP																		4
#define DALI_ADDRESSING															5
#define DALI_UTILS																	6
#define DALI_POLLINGS																7

#define	DALI_BUFFER_LENGHT													64

#define A_HANDLER_ADDR_DELETE_SHORT_ADDRESS					0x0001
#define A_HANDLER_ADDR_LINE_SCANNING								0x0002
#define A_HANDLER_ADDR_NEW_INITIALIZATION						0x0004
#define A_HANDLER_ADDR_PROGRAM_SHORT_ADDRESS				0x0008
#define A_HANDLER_ADDR_QUERY_CONTROL_GEAR						0x0010
#define A_HANDLER_ADDR_QUERY_GROUPS									0x0020
#define A_HANDLER_ADDR_QUERY_DEVICE_TYPE						0x0040
#define A_HANDLER_ADDR_SYSTEM_EXTENSION							0x0080
#define A_HANDLER_ADDR_SEARCH_RANDOM_ADDRESS				0x0100
#define A_HANDLER_ADDR_QUERY_SERIAL_NUMBER					0x0200



unsigned char daliAccessHandlerAddrLock (unsigned short int handler);
void daliAccessHandlerAddrUnlock (unsigned short int handler);




unsigned char daliSendInProgressGet(void);
static unsigned char daliPacketIdCntGet (void);
static void daliPacketIdCntInc (void);

unsigned char daliAccessHandlerAllowOnlyThisModule (unsigned char module, unsigned char stop, unsigned char wait);
void daliAccessHandlerAllowAllModules (void);


unsigned char daliSend(unsigned char module, unsigned int senderPacketId, unsigned char frameType, unsigned char dest, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow);
unsigned char daliBufferSend(unsigned char module, unsigned int senderPacketId, unsigned char frameType, unsigned char *dest,  unsigned char *com);

unsigned char daliDestCorrecto (unsigned char dest);

unsigned char daliPollingSend (unsigned char frameType, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow);
void daliStop (void);
void daliDoTask (void);
