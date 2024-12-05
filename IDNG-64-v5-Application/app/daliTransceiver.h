#define DALI_TRANSCEIVER_STATUS_READY   												0x00
#define DALI_TRANSCEIVER_STATUS_BUSY  												  0x01
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_0_DATA				0x01
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_1_DATA				0x03
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_N64_DATA			0x05
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_0_DATA				0x07
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_1_DATA				0x09
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_N64_DATA			0x11
#define DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_SNS_0_DATA		0x15

#define DALI_TRANSCEIVER_SPI_MAX_RETRIES												3
#define DALI_TRANSCEIVER_STATUS_BUSY_CNT_LIMIT									10000
#define DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_ADDRESS    0x00
#define DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER2_ADDRESS    0x01
#define DALI_TRANSCEIVER_FRAME_TYPE_ADDRESS                     0xC0
#define DALI_TRANSCEIVER_PACKET_ID_ADDRESS                      0xC1
#define DALI_TRANSCEIVER_DEST_ADDRESS                           0xC2
#define DALI_TRANSCEIVER_APPLY_7_ADDRESS                 				0xC3
#define DALI_TRANSCEIVER_COMMAND_HIGH_ADDRESS                   0xCB
#define DALI_TRANSCEIVER_COMMAND_LOW_ADDRESS                    0xCC
#define DALI_TRANSCEIVER_DATA_ADDRESS                    				0x7E
#define DALI_TRANSCEIVER_CRC_ADDRESS                            0xCD
#define DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_SEND				0x04
#define DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_STOP				0x02
#define DALI_TRANSCEIVER_FORWARD_LENGHT_ADDRESS									0xBF
#define DALI_TRANSCEIVER_FORWARD_DATA_0_ADDRESS									0xBE


// Almacena el frame recibido.
static void daliTransceiverIncomingFrameStore(unsigned char frameType, unsigned char packetId, unsigned char valid, unsigned char *validArray, unsigned char answer, unsigned char *answerArray, unsigned char sensorAddress);
static unsigned char daliTransceiverControlAndStatusRegisterGet(void);
static unsigned char daliTransceiverIncomingFrameCrcCheck (unsigned char frameType,unsigned char packetId,unsigned char valid,unsigned char validArray[8],unsigned char answer,unsigned char answerArray[64],unsigned char crc);
unsigned char daliTransceiverIncomingFrameGet (unsigned char *frameType, unsigned char *packetId, unsigned char *valid, unsigned char *validArray, unsigned char *answer, unsigned char *answerArray, unsigned char *sensorAddress);
unsigned char daliTransceiverStatusGet (void);
void daliTransceiverSend (unsigned char frameType, unsigned char packetId, unsigned char dest, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow, unsigned char data, unsigned char lenght, unsigned char *dataArray);
void daliTransceiverStop (void);
void daliTransceiverDoTask (void);
unsigned char daliTransceiverGetFwVer (void);


