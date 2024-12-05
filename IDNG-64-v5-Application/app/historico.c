#include "m25p.h"
#include "historico.h"
#include "macros.h"




unsigned char _historicoCurrentSector;									//0x00 ó 0x01
unsigned int  _historicoPtr;														//0x00 - 0x0FDF
unsigned char _historicoEstado;													//0x00 - LIBRE 		/		0x01 - LEYENDO.
unsigned int _historicoTicksCnt;


void historicoTick (void)
{
	if (_historicoEstado==0x01)
	{
		if (_historicoTicksCnt<HISTORICO_TIMEOUT_TICKS_LIMIT)
		{
			_historicoTicksCnt++;
		}
		else
		{
			_historicoEstado=0x00;
		}	
	}	
}


// Obtiene el numero de eventos que se pueden leer del histórico.
unsigned int historicoReadableEventsCntGet (void)
{
	unsigned char _flashMemPos[3];
	unsigned int ret=0;
	unsigned char completedPage;
	unsigned char completedByte;		
	if (_historicoCurrentSector==0x00)
	{
		completedPage=0x01;
	}
	else
	{
		completedPage=0x00;
	}
	historicoFlashMemPosGet (completedPage, HISTORICO_COMPLETED_PTR, &_flashMemPos[0]);
	m25pReadBytes(_flashMemPos[0],_flashMemPos[1],HISTORICO_COMPLETED_BYTE_PTR,&completedByte,1);			
	
	if (completedByte==0x01)
	{
		ret=4064+_historicoPtr;
	}		
	else
	{
		ret=_historicoPtr;
	}	
	return ret;	
}










// Escribe el mapa de utilización
void historicoMapPtrUsedSet (unsigned char currentSector, unsigned int ptr)
{
	
	unsigned char buffer;
	unsigned char page;
	unsigned char byte;
	unsigned char sector[2]={0x02,0x03};
	unsigned char status;	
	// 4064 eventos por sector -> Necesito 508 bytes, de las ultimas páginas FE y FF.	
	// evento 0-7   				-> fe 00
	// evento 8-15 					-> fe 01
	// evento 16-23 				-> fe 02	
	// evento 2024-2031			-> fe fd
	// evento 2032-2039			-> fe fe
	// evento 2040-2047			-> fe ff		
	// evento 2048-2055 		-> ff00
	// evento 4056-4063 		-> ffdf 
	// unused1							-> fffc
	// unused2							-> fffd
	// unused3							-> fffe
	// completed byte 			-> ffff	
	if (ptr<4064)
	{
		if (ptr<2048)
		{
			page=0xFE;
		}
		else
		{
			page=0xFF;
		}
		byte=(unsigned char)(ptr/0x0008);	
		m25pReadBytes(sector[currentSector],page,byte,&buffer,1);			
		CLEARBIT(buffer,(ptr-(8*byte)));		
		m25pWriteEnable();
		m25pWriteBytes(sector[currentSector],page,byte,&buffer,1);	
		do{		
		status=m25pStatusRead();
		}while((status&0x01)==0x01);			
	}
}	

// Lee un dato de memoria.
unsigned char historicoRead (unsigned int readPtr, unsigned char *data)
{
	unsigned char _flashMemPos[3];
	unsigned char ret=0;
	volatile unsigned int ptr;
	unsigned char completedPage;
	unsigned char completedByte;	
	_historicoEstado=0x01;
	_historicoTicksCnt=0;	
	if (_historicoPtr>readPtr)
	{
		ptr=_historicoPtr-readPtr-1;
		ret=historicoDataGet(_historicoCurrentSector,ptr,&data[0]);
	}
	else
	{		
		if (_historicoCurrentSector==0x00)
		{
			completedPage=0x01;
		}
		else
		{
			completedPage=0x00;
		}
		historicoFlashMemPosGet (completedPage, HISTORICO_COMPLETED_PTR, &_flashMemPos[0]);
		m25pReadBytes(_flashMemPos[0],_flashMemPos[1],HISTORICO_COMPLETED_BYTE_PTR,&completedByte,1);			
		if (completedByte==0x01)
		{
			ptr=HISTORICO_MAP_0_PTR-(readPtr-_historicoPtr+1);
			ret=historicoDataGet(completedPage,ptr,&data[0]);
		}		
		else
		{
			// Fuera de margen
		}
	}	
	return ret;
}

unsigned char historicoFlashMemPosGet (unsigned char currentPage, unsigned int ptr, unsigned char *flashMemPos)
{
	unsigned char page[2]={0x02,0x03};
	unsigned char ret;		
	if ((currentPage==0x00) || (currentPage==0x01))
	{
		flashMemPos[0]=page[currentPage];
		flashMemPos[1]=(unsigned char)(ptr/0x0010);
		flashMemPos[2]=(unsigned char)((ptr*0x0010)&(0x00FF));			
		ret=0x01;		
	}
	else
	{
		ret=0x00;
	}
	return ret;	
}


void historicoPtrInc(unsigned char *currentPage, unsigned int *ptr)
{
	unsigned char _flashMemPos[3];
	unsigned char completed=0x01;
	unsigned char status;	
	if (*ptr<HISTORICO_MAP_0_PTR)
	{
		(*ptr)++;
	}
	else
	{
		*ptr=0x0000;
		historicoFlashMemPosGet (_historicoCurrentSector, HISTORICO_COMPLETED_PTR, &_flashMemPos[0]);
		// Set current page as completed.
		m25pWriteEnable();
		m25pWriteBytes(_flashMemPos[0],_flashMemPos[1], HISTORICO_COMPLETED_BYTE_PTR,&completed,1);	
		do{		
		status=m25pStatusRead();
		}while((status&0x01)==0x01);				
		// Change page.
		if (_historicoCurrentSector==0x00)
		{
			_historicoCurrentSector=0x01;
		}
		else
		{
			_historicoCurrentSector=0x00;
		}									
	}	
}




// Obtiene los punteros Start y Stop al arrancar el micro.
void historicoInit (void)
{
	unsigned char _flashMemPos[3];
	unsigned char completedByteSector0;
	unsigned char completedByteSector1;		
	unsigned int i;
	unsigned int j;
	volatile unsigned int zeroCnt=0x0000;
	volatile unsigned int lastZeroPos=0x0000;
	unsigned char sectorMap[508];
		
	historicoFlashMemPosGet (0x00, HISTORICO_COMPLETED_PTR, &_flashMemPos[0]);
	m25pReadBytes(_flashMemPos[0],_flashMemPos[1],HISTORICO_COMPLETED_BYTE_PTR,&completedByteSector0,1);			
	historicoFlashMemPosGet (0x01, HISTORICO_COMPLETED_PTR, &_flashMemPos[0]);
	m25pReadBytes(_flashMemPos[0],_flashMemPos[1],HISTORICO_COMPLETED_BYTE_PTR,&completedByteSector1,1);			
	if (((completedByteSector0==0xFF) && (completedByteSector1==0xFF)) || ((completedByteSector0==0xFF) && (completedByteSector1==0x01)) || ((completedByteSector0==0x01) && (completedByteSector1==0xFF)))
	{
		if ((completedByteSector0==0xFF) && (completedByteSector1==0xFF))
		{
			_historicoCurrentSector=0x00;
		}			
		if ((completedByteSector0==0xFF) && (completedByteSector1==0x01))
		{
			_historicoCurrentSector=0x00;
		}			
		if ((completedByteSector0==0x01) && (completedByteSector1==0xFF))
		{
			_historicoCurrentSector=0x01;
		}						
		historicoFlashMemPosGet (_historicoCurrentSector, HISTORICO_MAP_0_PTR, &_flashMemPos[0]);
		m25pReadBytes(_flashMemPos[0],_flashMemPos[1],0,&sectorMap[0],508);							
		for (i=0;i<508;i++)
		{
			for (j=0;j<8;j++)
			{
				if (BITVAL(sectorMap[i],j)==0x00)
				{
					zeroCnt++;
					lastZeroPos=(i*8)+j;
				}
			}				
		}						
		if ((zeroCnt>0) && (zeroCnt!=(lastZeroPos+1)))
		{
				//ptrInitErr
		}
		else
		{
			_historicoPtr=zeroCnt;
		}	
	}
	else
	{
	 //pageInitErr
	}
}




// Reinicializa todo el historico
void historicoReset (void)
{
	unsigned char status;
	m25pWriteEnable();
	m25pSectorErase (0x02,0,0);
	do{
	status=m25pStatusRead();
	}while((status&0x01)==0x01);		
	m25pWriteEnable();
	m25pSectorErase (0x03,0,0);
	do{
	status=m25pStatusRead();
	}while((status&0x01)==0x01);			
	_historicoCurrentSector=0x00;
	_historicoPtr=0x0000;	
}

// Añade un evento al historico, devuelve 0x01 si ha podido añadirlo
unsigned char historicoAdd (unsigned char loop, unsigned char shortAddress, unsigned char day, unsigned char month, unsigned char year, unsigned char hour, unsigned char minutes, unsigned char event)
{
	unsigned char _buffer[16];
	unsigned char page[2]={0x02,0x03};
	unsigned char _status;
	unsigned char _flashMemPos[3];
	unsigned char ret;
	
	if (_historicoEstado==0x00)
	{
		_buffer[0]=0x01;							//0x01
		_buffer[1]=0xFF;							//LOOP | OUTPUT
		_buffer[2]=shortAddress;			//SHORTADDRESS
		_buffer[3]=day;								//DAY
		_buffer[4]=month;							//MONTH
		_buffer[5]=year;							//YEAR
		_buffer[6]=hour;							//HOUR
		_buffer[7]=minutes;						//MINUTES
		_buffer[8]=event;							//EVENT
		_buffer[9]=0xFF;							//SERIAL MSB
		_buffer[10]=0xFF;							//SERIAL 
		_buffer[11]=0xFF;							//SERIAL
		_buffer[12]=0xFF;							//SERIAL LSB
		_buffer[13]=0xFF;							//0xFF
		_buffer[14]=0xFF;							//0xFF
		_buffer[15]=0xFF;							//0xFF
		if (_historicoPtr==0x0000)
		{
			m25pWriteEnable();
			m25pSectorErase (page[_historicoCurrentSector],0,0);
			do{
			_status=m25pStatusRead();
			}while((_status&0x01)==0x01);				
		}
		if (historicoFlashMemPosGet (_historicoCurrentSector, _historicoPtr, &_flashMemPos[0])==0x01)								// En base a la página 0-1 y al puntero de evento, devuelve la posición en memoria H,M,L donde debo escribir.
		{
			m25pWriteEnable();
			m25pWriteBytes(_flashMemPos[0],_flashMemPos[1],_flashMemPos[2],&_buffer[0],16);	
			do{		
			_status=m25pStatusRead();
			}while((_status&0x01)==0x01);		
			historicoMapPtrUsedSet (_historicoCurrentSector, _historicoPtr);	
			historicoPtrInc(&_historicoCurrentSector, &_historicoPtr);																								// En base a la página 0-1 y al puntero de evento incrementa dicho puntero.													
		}
		ret=0x01;		
	}
	else
	{
		ret=0x00;
	}
	return ret;
}

unsigned char historicoDataGet (unsigned char page, unsigned int ptr, unsigned char *data)
{
	unsigned char _flashMemPos[3];
	unsigned char ret;
	
	if (historicoFlashMemPosGet (page, ptr, &_flashMemPos[0])==0x01)								// En base a la página 0-1 y al puntero de evento, devuelve la posición en memoria H,M,L donde debo escribir.
	{
		m25pReadBytes(_flashMemPos[0],_flashMemPos[1],_flashMemPos[2],&data[0],16);			
		if (data[0]==0x01)
		{
			ret=0x01;	
		}
		else
		{
			ret=0x00;
		}
	}
	else
	{
		ret=0x00;
	}
	return ret;
}










