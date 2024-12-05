// Comprobado 14/07/2016
// Posibles modificaciones pendientes: 
// El CRC no se está utilizando
// Handlers para debugging

#include "daliTransceiver.h"
#include "spi1.h"

unsigned char _daliTransceiverStatus;																					// Estado actual del transceptor DALI_TRANSCEIVER_STATUS_READY ó DALI_TRANSCEIVER_STATUS_BUSY
unsigned int 	_daliTransceiverStatusBusyCnt;																	// Contador de transceptor en estado DALI_TRANSCEIVER_STATUS_BUSY que sirve para liberarlo, limite definido en DALI_TRANSCEIVER_STATUS_BUSY_CNT_LIMIT
unsigned char _daliTransceiverDiscardFrame;																		// El frame recibido debe ser descartado
unsigned char _daliIncomingFramePending;																			// Se ha recibido un frame correctamente, está pendiente de que las capas superiores lo utilicen.
unsigned char _daliIncomingFrameFrameType;																		// Tipo de frame recibido
unsigned char _daliIncomingFramePacketId;																			// Packet ID del frame recibido
unsigned char _daliIncomingFrameValid;																				// Valid del frame recibido	
unsigned char	_daliIncomingFrameValidArray[8];																// ValidArray del frame recibido
unsigned char _daliIncomingFrameAnswer;																				// Answer del frame recibido
unsigned char _daliIncomingFrameAnswerArray[64];															// Answer array del frame recibido
unsigned char _daliIncomingFrameSensorAddress;
unsigned char _daliIncomingFrameCrc;																					// CRC del frame recibido

volatile unsigned char _cntdbg=0;

volatile unsigned char _casdbg;






// Almacena el frame recibido en _daliIncomingFrame.
static void daliTransceiverIncomingFrameStore(unsigned char frameType, unsigned char packetId, unsigned char valid, unsigned char *validArray, unsigned char answer, unsigned char *answerArray, unsigned char sensorAddress)
{
	unsigned char _n;
	_daliIncomingFramePending=0x01;
	_daliIncomingFrameFrameType=frameType;
	_daliIncomingFramePacketId=packetId;
	_daliIncomingFrameValid=valid;
	for (_n=0;_n<8;_n++)
	{
		_daliIncomingFrameValidArray[_n]=validArray[_n];
	}
	_daliIncomingFrameAnswer=answer;
	for (_n=0;_n<64;_n++)
	{
		_daliIncomingFrameAnswerArray[_n]=answerArray[_n];
	}	
	
	_daliIncomingFrameSensorAddress=sensorAddress;
}

// Obtiene el registro ControlAndStatusRegister del transceptor.
static unsigned char daliTransceiverControlAndStatusRegisterGet(void)
{
	unsigned char _answer;
	_answer=spi1Read(DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_ADDRESS);
	return _answer;	
}

// Obtiene el registro ControlAndStatusRegister del transceptor.
static unsigned char daliTransceiverControlAndStatusRegister2Get(void)
{
	unsigned char _answer;
	_answer=spi1Read(DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER2_ADDRESS);
	return _answer;	
}

// Comprueba el CRC del frame de entrada y devuelve 0x01 si es correcto.
static unsigned char daliTransceiverIncomingFrameCrcCheck (unsigned char frameType,unsigned char packetId,unsigned char valid,unsigned char validArray[8],unsigned char answer,unsigned char answerArray[64],unsigned char crc)
{
	return 0x01;
}
// Devuelve el frame recibido que previamente ha sido almacenado en _daliIncomingFrame.
unsigned char daliTransceiverIncomingFrameGet (unsigned char *frameType, unsigned char *packetId, unsigned char *valid, unsigned char *validArray, unsigned char *answer, unsigned char *answerArray, unsigned char *sensorAddress)
{	
	unsigned char _answer;	
	unsigned char _n;
	*frameType=	_daliIncomingFrameFrameType;
	*packetId = _daliIncomingFramePacketId;
	*valid = _daliIncomingFrameValid;
	for (_n=0;_n<8;_n++)
	{
		validArray[_n] = _daliIncomingFrameValidArray[_n];
	}
	*answer = _daliIncomingFrameAnswer;
	for (_n=0;_n<64;_n++)
	{	
		answerArray[_n] = _daliIncomingFrameAnswerArray[_n];
	}	
	*sensorAddress=_daliIncomingFrameSensorAddress;	
	_answer=_daliIncomingFramePending;
	_daliIncomingFramePending=0x00;
	return _answer;	
}


unsigned char daliTransceiverGetFwVer (void)
{
	unsigned char read[2];
	unsigned char answer;
	read[0]=spi1Read(0xEE);
	read[1]=(~(spi1Read(0xEF)));	
	if (read[0]==read[1])
	{
		answer=read[0];
	}	
	else
	{
		answer=0xFF;	
	}
	return answer;
}


// Devuelve el estado del transceptor DALI_TRANSCEIVER_STATUS_READY / DALI_TRANSCEIVER_STATUS_BUSY, que nos indica si es posible o no enviar.
unsigned char daliTransceiverStatusGet (void)
{
	return _daliTransceiverStatus;
}

void daliBufferTransceiverSend(unsigned char frameType, unsigned char packetId, unsigned char *dest, unsigned char *com)
{
		unsigned char n;
		unsigned char crc;
		botoneraLinkLedBlink(); 			
		_daliTransceiverStatus=DALI_TRANSCEIVER_STATUS_BUSY;																// Set TRANSCEIVER BUSY
		_daliTransceiverStatusBusyCnt=0;																										// Reset BUSY CNT
		_daliTransceiverDiscardFrame=0x00;
	  spi1Write(DALI_TRANSCEIVER_FRAME_TYPE_ADDRESS, frameType);
    spi1Write(DALI_TRANSCEIVER_PACKET_ID_ADDRESS, packetId);
		for (n=0;n<64;n++)
		{	
			spi1Write((0x0b+n), dest[n]);					
		}		
		for (n=0;n<64;n++)
		{	
			spi1Write((0x7f+n), com[n]);					
		}		
		spi1Write(DALI_TRANSCEIVER_CRC_ADDRESS, crc);	
    spi1Write(DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_ADDRESS,DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_SEND);     	
}



// Envia por el transceptor el frame solicitado.
void daliTransceiverSend (unsigned char frameType, unsigned char packetId, unsigned char dest, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow, unsigned char data, unsigned char lenght, unsigned char *dataArray)
{
	  unsigned char n;
		unsigned char crc;
		botoneraLinkLedBlink(); 			
		_daliTransceiverStatus=DALI_TRANSCEIVER_STATUS_BUSY;																// Set TRANSCEIVER BUSY
		_daliTransceiverStatusBusyCnt=0;																										// Reset BUSY CNT
		_daliTransceiverDiscardFrame=0x00;
	  spi1Write(DALI_TRANSCEIVER_FRAME_TYPE_ADDRESS, frameType);
    spi1Write(DALI_TRANSCEIVER_PACKET_ID_ADDRESS, packetId);	
		if ((frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_0_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_1_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_N64_DATA) || (frameType==0x16))
		{
				spi1Write(DALI_TRANSCEIVER_DEST_ADDRESS, dest);					
		}
		else
		{		
			if ((frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_0_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_1_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_N64_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_SNS_0_DATA))
			{
					for (n=0;n<8;n++)
					{
						spi1Write((DALI_TRANSCEIVER_APPLY_7_ADDRESS+n), apply[n]);
					}				
			}
		}		
		spi1Write(DALI_TRANSCEIVER_COMMAND_HIGH_ADDRESS, commandHigh);
    spi1Write(DALI_TRANSCEIVER_COMMAND_LOW_ADDRESS, commandLow);			
		if (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_1_DATA)
		{
			spi1Write(DALI_TRANSCEIVER_DATA_ADDRESS, data);						
		}
		else
		{
			if ((frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_SINGLE_N64_DATA) || (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_N64_DATA))
			{
					spi1Write(DALI_TRANSCEIVER_FORWARD_LENGHT_ADDRESS, lenght);
					for (n=0;n<lenght;n++)
					{
						spi1Write((DALI_TRANSCEIVER_FORWARD_DATA_0_ADDRESS-n), dataArray[n]);
					}
			}			
			else
			{
				if (frameType==DALI_TRANSCEIVER_FRAME_TYPE_FORWARD_BATCH_1_DATA)
				{
					for (n=0;n<64;n++)
					{
						spi1Write((DALI_TRANSCEIVER_FORWARD_DATA_0_ADDRESS-n), dataArray[n]);
					}
				}				
			}			
		}

		
		spi1Write(DALI_TRANSCEIVER_CRC_ADDRESS, crc);	
    spi1Write(DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_ADDRESS,DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_SEND);                     
}


// Detiene la operación en curso en el transceptor DALI.
void daliTransceiverStop (void)
{
		unsigned char cnt=0;
		unsigned char transceiverStatus;
		do
		{
			sysTickDelay(1);
			spi1Write(DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_ADDRESS, DALI_TRANSCEIVER_CONTROL_AND_STATUS_REGISTER_STOP);							
			sysTickDelay(1);
			transceiverStatus=daliTransceiverControlAndStatusRegister2Get();
			cnt++;	
		} while ((((transceiverStatus&16))==0) && (cnt<5));	
		_daliTransceiverDiscardFrame=0x01;
		
}

// Función de refresco del transceptor DALI.
void daliTransceiverDoTask (void)
{	
	unsigned char daliTransceiverControlAndStatusRegister;
	unsigned char frameType;
	unsigned char packetId;
	unsigned char valid;
	unsigned char validArray[8];
	unsigned char answer;
	unsigned char answerArray[64];
	unsigned char lenght;	
	unsigned char sensorAddress=0xFF;	
	unsigned char crc;
	unsigned char n;
	unsigned char retr=DALI_TRANSCEIVER_SPI_MAX_RETRIES;

	
	
	if (boardDALIISRGet()==1)																							// Hay interrupción
	{
		daliTransceiverControlAndStatusRegister=daliTransceiverControlAndStatusRegisterGet();				// Procesado	

		if ((daliTransceiverControlAndStatusRegister&0x08)==0x08)	//DALI_ERR
		{
			botoneraFailLedEncender();
		}
		else
		{
			botoneraFailLedApagar();			
			boardLedRojoOFF();
		}


		
		if ((daliTransceiverControlAndStatusRegister&0x01)==0x01)	//done
		{
			do
			{
				retr--;
				frameType=spi1Read (0x40);				// Get Frame Type
				packetId=spi1Read (0x41);					// Get Packet ID
				if (frameType==0x80)
				{
						valid=0;											// Get Valid
						for (n=0;n<8;n++)							// Get Valid Array
						{					
							validArray[7-n]=spi1Read(0x43+n);
						}
						answer=0;											// Get Answer
						for (n=0;n<64;n++)						// Get Answer Array
						{					
							answerArray[63-n]=spi1Read(0x4C+n);
						}
				}
				
				if (frameType==0x84)
				{
						valid=0;											// Get Valid
						for (n=0;n<8;n++)							// Get Valid Array
						{					
							validArray[7-n]=spi1Read(0x43+n);
						}
						answer=0;											// Get Answer
						for (n=0;n<64;n++)						// Get Answer Array
						{					
							answerArray[63-n]=spi1Read(0x4C+n);
						}						
						sensorAddress=spi1Read(0x8E);
						
						if (sensorAddress==255)
						{
							frameType=0x80;
						}
						
				}
				
				
				if (frameType==0x82)
				{
						valid=spi1Read (0x42);				// Get Valid
						for (n=0;n<8;n++)							// Get Valid Array
						{					
							validArray[n]=0;
						}
						answer=spi1Read(0x4B);				// Get Answer
						for (n=0;n<64;n++)						// Get Answer Array
						{					
							answerArray[n]=0;
						}
				}				
				if (frameType==0x83)
				{
						valid=spi1Read (0x42);				// Get Valid
						for (n=0;n<8;n++)							// Get Valid Array
						{					
							validArray[n]=0;
						}
						lenght=spi1Read(0x8E);
						for (n=0;n<lenght;n++)
						{
							answerArray[n]=spi1Read(0x8C-n);
						}
				}
				
		
				
				
				crc=spi1Read(0x8D);				
			}while ((daliTransceiverIncomingFrameCrcCheck (frameType,packetId,valid,validArray,answer,answerArray,crc)==0x00) && (retr>0));	
			if (retr>0)	
			{
				if (_daliTransceiverDiscardFrame==0x00)
				{				
					daliTransceiverIncomingFrameStore(frameType,packetId,valid,&validArray[0],answer,&answerArray[0],sensorAddress);				
				}
				else
				{
					if (frameType==0x80)	frameType=0x99;					
					daliTransceiverIncomingFrameStore(frameType,packetId,valid,&validArray[0],answer,&answerArray[0],sensorAddress);		
					
					// Frame descartado
				}
			}
			else
			{	// Se ha superado el número máximo de reintentos y el paquete sigue sin ser correcto.
			}
			_daliTransceiverStatus=DALI_TRANSCEIVER_STATUS_READY;	
			polling_BusyClear();
			boardLedVerdeOFF();		
		}
		else
		{	//Interrupción generada y el motivo no es que la operación esté realizada.
		}
	}	
	// Liberación del transceiver en caso de que no se reciba nada.
	if (_daliTransceiverStatus==DALI_TRANSCEIVER_STATUS_BUSY)																			
	{		
		if (_daliTransceiverStatusBusyCnt<DALI_TRANSCEIVER_STATUS_BUSY_CNT_LIMIT)
		{
			_daliTransceiverStatusBusyCnt++;
		}
		else
		{
			// Liberación forzosa del transceiver [Timeout]
			_daliTransceiverStatus=DALI_TRANSCEIVER_STATUS_READY;		
			polling_BusyClear();
		}	
	}	
}
