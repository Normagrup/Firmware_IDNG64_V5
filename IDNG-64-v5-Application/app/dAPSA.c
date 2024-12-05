		#include "dali.h"

#define DAQCGL_TIMEOUT			15
#define NOK         				0x00
#define OK         					0x01
#define NA         					0xFF		
#define ERR_TIMEOUT					0x02			
#define TRUE                0x01
#define FALSE               0x00
#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02







#define STAGE_0             0x00
#define STAGE_1             0x01
#define STAGE_2             0x02
#define STAGE_3             0x03
#define STAGE_4             0x04
#define STAGE_5             0x05
#define STAGE_6             0x06
#define STAGE_7             0x07
#define STAGE_8             0x08
#define STAGE_9             0x09
#define STAGE_10            0x0A
#define STAGE_11            0x0B
#define STAGE_12            0x0C
#define STAGE_13            0x0D
#define STAGE_14            0x0E
#define STAGE_15            0x0F














#define BOOL                unsigned char	

// Externas.

extern unsigned char		_dATerminateSent;
extern unsigned char		_dAInitializeSent;
extern unsigned char		_dASearchAddrHSent;
extern unsigned char		_dASearchAddrMSent;
extern unsigned char		_dASearchAddrLSent;
extern unsigned char		_dAQuerySAddrSent;
extern unsigned char 		_dAProgramSAddrSent;

extern BOOL							_dAQuerySAddrReplyValid;

extern unsigned char 									_dASRAEncontradosList[4][64];                                   // Salida		

extern unsigned char									_dAQCGLDireccionesOcupadasMatriz[64];


// Internas.





unsigned char   				_dAPSAStage;
unsigned char   				_dAPSAInitializeMode;																						// Varible utilizada para el algoritmo de búsqueda.
//unsigned char						_dAPSAGlobalProcTimeout;
//unsigned char						_dAPSADeviceProcTimeout;    
unsigned char   				_dAPSAStatus;																										// Salida
unsigned char   				_dAPSAResult;																										// Salida	
unsigned char   				_dAPSAExitCode;																									// Salida	
unsigned char						_dAPSAEquiposCnt;
unsigned char						_dAPSAEquiposCntLimit;
unsigned char						_dAPSADireccionesLibresList[64];															// Aquí grabarías en otro formato. ej 0,2,3,4,FF,FF,FF
unsigned char						_dAPSAReintentosCnt;

unsigned char dASRAEncontradosHGet(unsigned char equipo);
unsigned char dASRAEncontradosMGet(unsigned char equipo);
unsigned char dASRAEncontradosLGet(unsigned char equipo);
				
static void dAPSAGotoStage (unsigned char stage)
{	
	_dATerminateSent=NOT_SENDING;
	_dAInitializeSent=NOT_SENDING;
	_dASearchAddrHSent=NOT_SENDING;
	_dASearchAddrMSent=NOT_SENDING;
	_dASearchAddrLSent=NOT_SENDING;
	_dAQuerySAddrSent=NOT_SENDING;	
	_dAProgramSAddrSent=NOT_SENDING;
	_dAPSAStage=stage;
}

void dAPSARtcTick (void)
{
	if (_dAPSAStatus==IN_PROGRESS)
	{		
	}
}

BOOL dAPSAStart (unsigned char initializeMode, unsigned char equiposCntLimit)
{	
	unsigned char n;
	unsigned char i;
	unsigned char j;	
	unsigned char retVal;	
	if (_dAPSAStatus!=IN_PROGRESS)
	{	
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_PROGRAM_SHORT_ADDRESS)==0x01)
		{		
			_dAPSAReintentosCnt=3;
			_dAPSAInitializeMode=initializeMode;
//		_dAPSARandomAddressListPtr=randomAddressListPtr;	
			_dAPSAEquiposCnt=0;	
			_dAPSAEquiposCntLimit=equiposCntLimit;	
			for (n=0;n<64;n++)
			{
				_dAPSADireccionesLibresList[n]=0xFF;		
			}	
			j=0;
			for (i=0;i<64;i++)
			{
				if (_dAQCGLDireccionesOcupadasMatriz[i]==0x00)	
				{
					_dAPSADireccionesLibresList[j]=i;
					j++;
				}		
			}			
			_dAPSAStatus=IN_PROGRESS;
			dAPSAGotoStage(STAGE_0);			
			retVal=TRUE;
		}	
		else
		{
			retVal=FALSE;
			_dAPSAStatus=DONE;
		}
	}
	else
	{
		retVal=FALSE;
		_dAPSAStatus=DONE;
	}
}

void dAPSAStop (void)
{	
}

void dAPSADoTask (void)
{
	unsigned char h;
	unsigned char m;
	unsigned char l;
	if (_dAPSAStatus==IN_PROGRESS)
	{		
		if (_dAPSAStage==STAGE_0)
		{
			daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x02,0x00);
			dAPSAGotoStage(STAGE_1);
		}		
		if (_dAPSAStage==STAGE_1)
		{
			if (_dATerminateSent==NOT_SENDING)       				dATerminateSend();
			if (_dATerminateSent==SENT)              				dAInitializeSend(_dAPSAInitializeMode);			
			if (_dAInitializeSent==SENT)
			{
				dAPSAGotoStage(STAGE_2);
			}			
		}				
		if (_dAPSAStage==STAGE_2)
		{
			
				h=dASRAEncontradosHGet(_dAPSAEquiposCnt);
				m=dASRAEncontradosMGet(_dAPSAEquiposCnt);
				l=dASRAEncontradosLGet(_dAPSAEquiposCnt);
			
			if (_dASearchAddrHSent==NOT_SENDING)           dASearchAddrHSend(h);
			if (_dASearchAddrHSent==SENT)                  dASearchAddrMSend(m);
			if (_dASearchAddrMSent==SENT)                  dASearchAddrLSend(l);
			if (_dASearchAddrLSent==SENT)									 dAQueryShortAddrSend();
			if (_dAQuerySAddrSent==SENT)
			{
				if (_dAQuerySAddrReplyValid==FALSE)
				{
					if (_dAPSAReintentosCnt>0)
					{
						daliAddressingDebug(0x94,h,m,l,0xFF,_dAPSAEquiposCnt);
						_dAPSAReintentosCnt--;
						dAPSAGotoStage(STAGE_2);						
					}
					else
					{						
						// EXCESO DE REINTENTOS PARA GRABAR UNA DIRECCION.
						_dAPSAResult=NOK;
						_dAPSAExitCode=NA;								
						dAPSAGotoStage(STAGE_5);												
					}				
				}
				else
				{
					dAPSAGotoStage(STAGE_3);	
				}				
			}			
		}	
		if (_dAPSAStage==STAGE_3)
		{				
			if (_dAProgramSAddrSent==NOT_SENDING)						dAProgramSAddrSend((_dAPSADireccionesLibresList[_dAPSAEquiposCnt]<<1)+1);			
			if (_dAProgramSAddrSent==SENT)									dAQueryShortAddrSend();
			if (_dAQuerySAddrSent==SENT)
			{			
				if (_dAQuerySAddrReplyValid==FALSE)
				{				
					if (_dAPSAReintentosCnt>0)
					{						
						daliAddressingDebug(0x95,h,m,l,0xFF,_dAPSAEquiposCnt);
						_dAPSAReintentosCnt--;
						dAPSAGotoStage(STAGE_2);						
					}
					else
					{
						// EXCESO DE REINTENTOS PARA GRABAR UNA DIRECCION.
						_dAPSAResult=NOK;
						_dAPSAExitCode=NA;								
						dAPSAGotoStage(STAGE_5);	
					}
				}
				else
				{
					dAPSAGotoStage(STAGE_4);						
				}
			}
		}			
		if (_dAPSAStage==STAGE_4)
		{
			_dAPSAEquiposCnt++;
			daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x02,_dAPSAEquiposCnt);
			if (_dAPSAEquiposCnt<_dAPSAEquiposCntLimit)
			{				
				dAPSAGotoStage(STAGE_2);
			}
			else
			{
				_dAPSAResult=OK;
				_dAPSAExitCode=NA;
				dAPSAGotoStage(STAGE_5);
			}	
		}	
		if (_dAPSAStage==STAGE_5)
		{
			if (_dATerminateSent==NOT_SENDING)                   dATerminateSend();
			if (_dATerminateSent==SENT)
			{
				daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_PROGRAM_SHORT_ADDRESS);
				_dAPSAStatus=DONE;
			}
		}			
	}
}

void dAPSADoneFlagClear (void)
{
	_dAPSAStatus=NOT_SENDING;
}