#include "daliCommands.h"
#include "dali.h"
#include "daliLoop.h"
#include "config.h"
#include "qSN.h"




#define ALL_DEVICES         0x00
#define NEW_DEVICES         0xFF
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

// >> VARIABLES RELACIONADAS CON EL ENVIO DE COMANDOS.
    unsigned char    _dASearchAddrHSent;
    unsigned char    _dASearchAddrMSent;
    unsigned char    _dASearchAddrLSent;
    unsigned char    _dACompareSent;
    BOOL             _dACompareResult;
    unsigned char    _dAQueryCGLoopSent;
    unsigned char    _dAQueryMissingSAddrSent;
    BOOL             _dAQueryMissingSAddrResult;
    unsigned char    _dAInitializeSent;
    unsigned char    _dAWithdrawSent;
    unsigned char    _dATerminateSent;
    unsigned char    _dARandomizeSent;
    unsigned char    _dAQuerySAddrSent;
    BOOL             _dAQuerySAddrReplyValid;
    unsigned char    _dAQuerySAddrReplyAnswer;
		unsigned char		_dAProgramSAddrSent;
		unsigned char		_dADTRSent;		
		unsigned char		_dAStoreDTRAsAddrSent;		
		unsigned char 	_dAQueryDTLoopSent;
		
		unsigned char _dAQueryDTLoopSentMulti;
		unsigned char _dAQueryDTSent;
		
		unsigned char		_newInitType;

		

// << VARIABLES RELACIONADAS CON EL ENVIO DE COMANDOS.

// << VARIABLES OTRAS.
    unsigned char queryMissingShortAddress;
    unsigned char _matrizOcupacionDirecciones[64];
    unsigned char ocupacionDireccionesCnt;
    unsigned char daliAddressingSystemExtensionStatus;
    unsigned char daliAddressingSystemExtensionStage;
    unsigned char programAddress;
// >> VARIABLES OTRAS.


		// >> _ d A S R A ...		
			// >> Variables.
				extern unsigned char   				_dASRAStatus;																										// Salida
				extern unsigned char   				_dASRAResult;																										// Salida	
				extern unsigned char   				_dASRAExitCode;																									// Salida	
				extern unsigned char 					_dASRAEncontradosCnt;                                           // Salida
				extern unsigned char 					_dASRAEncontradosList[4][64];                                   // Salida		
			// << Variables.
			// >> Prototipos.		
				void dASRARtcTick (void);
				static void	dASRAGotoStage (unsigned char stage);
				BOOL dASRAStart (unsigned char initializeMode, BOOL randomizeIsNeeded);
				void dASRAStop (void);
				void dASRADoneFlagClear (void);
				void dASRADoTask (void);
			// << Prototipos.
	// << _ d A S R A ...			
	
	// >> _ d A Q C G L ...
		// >> Variables.
			extern unsigned char 					_dAQCGLStatus;			
			extern unsigned char          _dAQCGLDireccionesOcupadasMatriz[64];
			extern unsigned char 					_dAQCGLDireccionesOcupadasCnt;
			
			
			
			extern unsigned char					_dAQDTLTiposDispositivoMatriz[64];
			extern unsigned char					_dAQDTLTiposDispositivoValid[8];
			
			extern unsigned char _dAQGroupsLowStatus;
			extern unsigned char _dAQGroupsHighStatus;
			
			
		// << Variables.		
		// >> Prototipos.
			void dAQCGLDoneFlagClear (void);
			void dAQCGLRtcTick(void);
			BOOL queryControlGearLoopStart (void);
			void dAQCGLDoTask(void);
		// << Prototipos.		
	// >> _ d A Q C G L	...
		
		
		unsigned char _comA[2];		
		unsigned char _ipA[4];
		
		
		BOOL dALSStart(void);
		

    // >> COMANDOS.
    void dAQueryShortAddrSend (void)
    {
        unsigned char apply[8];
        if (_dAQuerySAddrSent==NOT_SENDING)
        {				
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xBB, &apply[0], 0x2E, 0)==0x01)
							{
									_dAQuerySAddrReplyValid=FALSE;
									_dAQuerySAddrReplyAnswer=0xFF;
									_dAQuerySAddrSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xBB, &apply[0], 0x2E, 0)==0x01)
							{
									_dAQuerySAddrReplyValid=FALSE;
									_dAQuerySAddrReplyAnswer=0xFF;
									_dAQuerySAddrSent=SENDING;
							}
						}
        }
    }
    void dAQueryShortAddrCb (unsigned char daliSentDest, unsigned char daliReplyValid, unsigned char daliReplyAnswer)
    {
        if ((_dAQuerySAddrSent==SENDING) && (daliSentDest==0xBB))
        {
            if (daliReplyValid==0x01)
            {
                _dAQuerySAddrReplyValid=TRUE;
                _dAQuerySAddrReplyAnswer=daliReplyAnswer;
            }
            _dAQuerySAddrSent=SENT;
        }
    }
    void dAInitializeSend (unsigned char parameter)
    {
        unsigned char apply[8];
        if (_dAInitializeSent==NOT_SENDING)
        {
            if (_newInitType==0x01)
						{	
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xA5, &apply[0], 0x2e, parameter)==0x01)
							{
									_dAInitializeSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA5, &apply[0], 0x2D, parameter)==0x01)
							{
									_dAInitializeSent=SENDING;
							}
						}
        }
    }
    void dAInitializeCb (unsigned char daliSentDest)
    {
        if ((_dAInitializeSent==SENDING) && (daliSentDest==0xA5))
        {
            _dAInitializeSent=SENT;
        }
    }
    void dARandomizeSend (void)
    {
        unsigned char apply[8];
        if (_dARandomizeSent==NOT_SENDING)
        {
            if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xA7, &apply[0], 0x2e, 0)==0x01)
							{
									_dARandomizeSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA7, &apply[0], 0x2D, 0)==0x01)
							{
									_dARandomizeSent=SENDING;
							}	
						}
        }
    }
    void dARandomizeCb (unsigned char daliSentDest)
    {
        if ((_dARandomizeSent==SENDING) && (daliSentDest==0xA7))
        {
            _dARandomizeSent=SENT;
        }
    }
    void dAWithdrawSend (void)
    {
        unsigned char apply[8];
        if  (_dAWithdrawSent==NOT_SENDING)
        {
            if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xAB, &apply[0], 0x2e, 0x00)==0x01)
							{
									_dAWithdrawSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xAB, &apply[0], 0x2B, 0x00)==0x01)
							{
									_dAWithdrawSent=SENDING;
							}
						}
					

        }
    }
    void dAWithdrawCb (unsigned char daliSentDest)
    {
        if ((_dAWithdrawSent==SENDING) && (daliSentDest==0xAB))
        {
            _dAWithdrawSent=SENT;
        }
    }
    void dATerminateSend (void)
    {
        unsigned char apply[8];
        if  (_dATerminateSent==NOT_SENDING)
        {
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xA1, &apply[0], 0x2E, 0)==0x01)
							{
									_dATerminateSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA1, &apply[0], 0x2B, 0)==0x01)
							{
									_dATerminateSent=SENDING;
							}							
						}
        }
    }
    void dATerminateCb (unsigned char daliSentDest)
    {
        if ((_dATerminateSent==SENDING) && (daliSentDest==0xA1))
        {
            _dATerminateSent=SENT;
        }
    }
    void dAQueryMissingSAddrSend (void)
    {
        unsigned char apply[8];
			
				
				if (_dAQueryMissingSAddrSent==NOT_SENDING)
				{
					if (_newInitType==0x01)
					{
						sysTickDelay (10);																		
						if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xFF, &apply[0], 0x2E, 0x96)==0x01)
						{
							_dAQueryMissingSAddrSent=SENDING;
						}						
					}
					else
					{
						if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2E, 0x96)==0x01)
						{
							_dAQueryMissingSAddrSent=SENDING;
						}
					}
				}
    }
    void dAQueryMissingSAddrCb (unsigned char daliSentCommandLow, unsigned char daliReplyValid, unsigned char daliReplyAnswer)
    {
        if ((_dAQueryMissingSAddrSent==SENDING) && (daliSentCommandLow==0x96))
        {
            if ((daliReplyValid==0x01) || ((daliReplyValid==0x00)  && (daliReplyAnswer==0x02)))
            {
                _dAQueryMissingSAddrResult=TRUE;
            }
            else
            {
                _dAQueryMissingSAddrResult=FALSE;
            }
            _dAQueryMissingSAddrSent=SENT;
        }
    }
    void dASearchAddrHSend (unsigned char value)
    {
        unsigned char apply[8];

			
				if (_dASearchAddrHSent==NOT_SENDING)
        {
					if (_newInitType==0x01)
					{	
						sysTickDelay (10);
						if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xB1, &apply[0], 0x2e, value)==0x01)
						{
                _dASearchAddrHSent=SENDING;
            }
					}
					else
					{
						if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xB1, &apply[0], 0x2B, value)==0x01)
            {
                _dASearchAddrHSent=SENDING;
            }
					}
        }
    }
    void dASearchAddrHCb (unsigned char daliSentDest)
    {
        if ((_dASearchAddrHSent==SENDING) && (daliSentDest==0xB1))
        {
            _dASearchAddrHSent=SENT;
        }
    }
    void dASearchAddrMSend (unsigned char value)
    {
        unsigned char apply[8];
        if (_dASearchAddrMSent==NOT_SENDING)
        {
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xB3, &apply[0], 0x2e, value)==0x01) 
							{
									_dASearchAddrMSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xB3, &apply[0], 0x2B, value)==0x01)
							{
									_dASearchAddrMSent=SENDING;
							}
						}
        }
    }
    void dASearchAddrMCb (unsigned char daliSentDest)
    {
        if ((_dASearchAddrMSent==SENDING) && (daliSentDest==0xB3))
        {
            _dASearchAddrMSent=SENT;
        }
    }
    void dASearchAddrLSend (unsigned char value)
    {
        unsigned char apply[8];
        if (_dASearchAddrLSent==NOT_SENDING)
        {
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xB5, &apply[0], 0x2e, value)==0x01)
							{
								_dASearchAddrLSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xB5, &apply[0], 0x2B, value)==0x01)
							{
								_dASearchAddrLSent=SENDING;
							}
						}
        }
    }
    void dASearchAddrLCb (unsigned char daliSentDest)
    {
        if ((_dASearchAddrLSent==SENDING) && (daliSentDest==0xB5))
        {
            _dASearchAddrLSent=SENT;
        }
    }
    void dACompareSend (void)
    {
        unsigned char apply[8];
        if (_dACompareSent==NOT_SENDING)
        {
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xA9, &apply[0], 0x2e, 0)==0x01)
							{
									_dACompareResult=FALSE;
									_dACompareSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA9, &apply[0], 0x2B, 0)==0x01)
							{
									_dACompareResult=FALSE;
									_dACompareSent=SENDING;
							}
						}
        }
    }
    void dACompareCb (unsigned char daliSentDest, unsigned char daliReplyValid, unsigned char daliReplyAnswer)
    {
        if ((_dACompareSent==SENDING) && (daliSentDest==0xA9))
        {
            if (_dACompareSent==SENDING)
            {
                if ((daliReplyValid==0x01) || ((daliReplyValid==0x00)  && (daliReplyAnswer==0x02)))
                {
                _dACompareResult=TRUE;
                }
                else
                {
                _dACompareResult=FALSE;
                }
                _dACompareSent=SENT;
            }
        }
    }
    void dAQueryCGLoopCb (unsigned char daliSentCommandLow, unsigned char *daliReplyValidArray, unsigned char *daliReplyAnswerArray)
    {
        unsigned char n;
        if ((_dAQueryCGLoopSent==SENDING) && (daliSentCommandLow==0x91))
        {
            ocupacionDireccionesCnt=0;
            for (n=0;n<64;n++)
            {
                if ((daliLoopDeviceBitFromByteArrayGet (&daliReplyValidArray[0],n)==0x01) && (daliReplyAnswerArray[n]==0xFF))
                {
                    _dAQCGLDireccionesOcupadasMatriz[n]=0x01;
                    _dAQCGLDireccionesOcupadasCnt++;
                }
                else
                {
                    _dAQCGLDireccionesOcupadasMatriz[n]=0x00;
                }
            }
            _dAQueryCGLoopSent=SENT;
        }
    }
    void dAQueryCGLoopSend (void)
    {
        unsigned char apply[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        if (_dAQueryCGLoopSent==NOT_SENDING)
        {
            if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x07, 0xFF, &apply[0], 0x2E, 0x91)==0x01)
            {
                _dAQueryCGLoopSent=SENDING;
            }
        }
    }	
		
		void dAQueryDTCb(unsigned char daliSentCommandLow, unsigned char daliSentDest, unsigned char daliReplyValid, unsigned char daliReplyAnswer)
		{
			unsigned char address;	
			unsigned char apply[8];			
			unsigned char reenvia=0;			
			if (_dAQueryDTSent==SENDING)
			{				
				address=((daliSentDest&0x7E)>>1);				
				if (daliSentCommandLow==0x99)				// QUERY DEVICE TYPE.
				{
					if (daliReplyValid==1)
					{	
						_dAQDTLTiposDispositivoMatriz[address]=daliReplyAnswer;
						if (daliReplyAnswer==255)
						{
							reenvia=1;
						}							
					}
					else
					{
						_dAQDTLTiposDispositivoMatriz[address]=255;
					}
				}
				else if	(daliSentCommandLow==0xA7)	// QUERY NEXT DEVICE TYPE.
				{
					reenvia=1;					
					if (daliReplyAnswer==1)																																																		_dAQDTLTiposDispositivoMatriz[address]=1;
					if ((daliReplyAnswer==6) && (_dAQDTLTiposDispositivoMatriz[address]!=8) && (_dAQDTLTiposDispositivoMatriz[address]!=1))		_dAQDTLTiposDispositivoMatriz[address]=6;
					if ((daliReplyAnswer==8) && (_dAQDTLTiposDispositivoMatriz[address]!=1))																									_dAQDTLTiposDispositivoMatriz[address]=8;
					if (daliReplyAnswer==254)																																																	reenvia=0;
				}				
				
				if (reenvia==1)
				{
					daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, daliSentDest, &apply[0], 0x2E, 0xA7);
				}
				else
				{
					_dAQueryDTSent=SENT;
				}
			}					
		}
		
		void dAQueryDTSend (unsigned char address)
    {
        unsigned char apply[8];
				if ((_dAQueryDTSent==NOT_SENDING) && (address<64))
        {				
					if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, ((address<<1)+1), &apply[0], 0x2E, 0x99)==0x01)
					{
						_dAQueryDTSent=SENDING;
          }
        }
    }		
		
		
    
				
		
		
		
		
		void dAProgramSAddrCb (unsigned char daliSentDest)
    {     
			if ((_dAProgramSAddrSent==SENDING) && (daliSentDest==0xB7))
			{
				_dAProgramSAddrSent=SENT;
			}
    }
    
		
		void dAProgramSAddrSend (unsigned char address)
		{
				unsigned char apply[8];
        if (_dAProgramSAddrSent==NOT_SENDING)
        {						
						if (_newInitType==0x01)
						{
							sysTickDelay (10);
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xB7, &apply[0], 0x2e, address)==0x01)
							{

									_dAProgramSAddrSent=SENDING;
							}
						}
						else
						{
							if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xB7, &apply[0], 0x2B, address)==0x01)
							{

									_dAProgramSAddrSent=SENDING;
							}
						}
        }
		}
		
		
		
		
		
		void dADTRCb (unsigned char daliSentDest)
		{
			if ((_dADTRSent==SENDING) && (daliSentDest==0xA3))
			{
				_dADTRSent=SENT;
			}
		}
		
		void dADTRSend (unsigned char data)
		{
			unsigned char apply[8];	
			if (_dADTRSent==NOT_SENDING)
			{						
				if (_newInitType==0x01)
				{
					_dADTRSent=SENT;
					//if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x16, 0xA3, &apply[0], 0x2e, data)==0x01)
					//{	
						//_dADTRSent=SENDING;		
					//}
				}
				else
				{
					if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA3, &apply[0], 0x2B, data)==0x01)
					{	
						_dADTRSent=SENDING;		
					}
				}
			}			
		}		

		void dAStoreDTRAsAddrCb (unsigned char daliSentCommandLow)
		{
			if ((_dAStoreDTRAsAddrSent==SENDING) && (daliSentCommandLow==0x80))
			{
				_dAStoreDTRAsAddrSent=SENT;
			}
		}
		
		void dAStoreDTRAsAddrSend (unsigned char dest)
		{									
			unsigned char apply[8];			
			if (_dAStoreDTRAsAddrSent==NOT_SENDING)
			{
				if (_newInitType==0x01)
				{
					_dAStoreDTRAsAddrSent=SENT;
				}
				else
				{
					if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, dest, &apply[0], 0x2D, 0x80)==0x01)
					{					
						_dAStoreDTRAsAddrSent=SENDING;
					}	
				}
			}								
		}
		
		
		void dAQGroupsLowSend(void)
		{
			  unsigned char apply[8];
				daliLoopInstalledArrayByteGet (&apply[0]);
				if (_dAQGroupsLowStatus==NOT_SENDING)
        {
            if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x07, 0xFF, &apply[0], 0x2E, 0xC0)==0x01)
            {
                _dAQGroupsLowStatus=SENDING;
            }
        }
		}
		
		void dAQGroupsHighSend(void)
		{
			unsigned char apply[8];
			daliLoopInstalledArrayByteGet (&apply[0]);  
			if (_dAQGroupsHighStatus==NOT_SENDING)
        {
            if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x07, 0xFF, &apply[0], 0x2E, 0xC1)==0x01)
            {
                _dAQGroupsHighStatus=SENDING;
            }
        }
		}
		
		void dAQGroupsLowCb (unsigned char daliSentCommandLow, unsigned char *daliReplyValidArray, unsigned char *daliReplyAnswerArray)
    {	
				unsigned char n;
			
        if ((_dAQGroupsLowStatus==SENDING) && (daliSentCommandLow==0xC0))
        {									
          configDaliLoopGroupLowSet(&daliReplyValidArray[0],&daliReplyAnswerArray[0],0x01); 
					_dAQGroupsLowStatus=SENT;
        }
				
    }
		
		void dAQGroupsHighCb (unsigned char daliSentCommandLow, unsigned char *daliReplyValidArray, unsigned char *daliReplyAnswerArray)
    {
        unsigned char n;
			
        if ((_dAQGroupsHighStatus==SENDING) && (daliSentCommandLow==0xC1))
        {					
          configDaliLoopGroupHighSet(&daliReplyValidArray[0],&daliReplyAnswerArray[0],0x01); 
					_dAQGroupsHighStatus=SENT;
        }
				
    }
		
		
		
		
		
		
		// << COMANDOS.




		
		

void daliAddressingNotify(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6)
{	
	unsigned char respuesta[10];	
	unsigned char ipDebug[4]={192,168,51,246};
	//BYTE1 -> NO VA A INICIAR LA OPERACION / VA A INICIAR LA OPERACION O LA ESTÁ HACIENDO.
	//BYTE2 -> VA A INICIAR LA OPERACION / ESTÁ HACIENDO LA OPERACION / FINALIZÓ LA OPERACIÓN.
	//BYTE3 -> NOK / OK.
	//BYTE4 -> INFO ERROR.
	//BYTE5 -> SUBOPERACION.
	//BYTE6 -> NUMERO EQUIPOS.	
		respuesta[0]=0x83;
		respuesta[1]=_comA[0];
		respuesta[2]=_comA[1];
		respuesta[3]=0x05;		
		respuesta[4]=byte1;
		respuesta[5]=byte2;
		respuesta[6]=byte3;
		respuesta[7]=byte4;
		respuesta[8]=byte5;
		respuesta[9]=byte6;
		ethSendFrame(0,&respuesta[0],10,&_ipA[0], 2234);			
		//ethSendFrame(0,&respuesta[0],10,&ipDebug[0], 2234);
}


		
void daliAddressingDebug(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6)
{	
	unsigned char respuesta[10];	
	unsigned char ipDebug[4]={192,168,51,255};
	//BYTE1 -> NO VA A INICIAR LA OPERACION / VA A INICIAR LA OPERACION O LA ESTÁ HACIENDO.
	//BYTE2 -> VA A INICIAR LA OPERACION / ESTÁ HACIENDO LA OPERACION / FINALIZÓ LA OPERACIÓN.
	//BYTE3 -> NOK / OK.
	//BYTE4 -> INFO ERROR.
	//BYTE5 -> SUBOPERACION.
	//BYTE6 -> NUMERO EQUIPOS.	
		respuesta[0]=0x83;
		respuesta[1]=_comA[0];
		respuesta[2]=_comA[1];
		respuesta[3]=0x05;		
		respuesta[4]=byte1;
		respuesta[5]=byte2;
		respuesta[6]=byte3;
		respuesta[7]=byte4;
		respuesta[8]=byte5;
		respuesta[9]=byte6;
		//ethSendFrame(0,&respuesta[0],10,&_ipA[0], 2235);		
		ethSendFrame(0,&respuesta[0],10,&ipDebug[0], 2235);		
}		
		
		
		
/*

// TMR FLANCO.

unsigned char				_tmrFlancoPinEstadoTmp;				// Flanco aún no confirmado. Pin Estado.
unsigned short int	_tmrFlancoTmrBitCntTmp;				// Flanco aún no confirmado. TmrBit.


void isrTmrFlanco (void)
{
	// Detiene el timer.
	// Detiene la interrupción asociada.
	confirmarFlanco();
}
		
void startTmrFlanco (unsigned char pinEstado, unsigned short int tmrBitCnt)
{
	// Precarga el timer.
	// Configurar la interrupción asociada.
	// Pone en marcha el timer.	
	_tmrFlancoPinEstadoTmp=pinEstado;
	_tmrFlancoTmrBitCntTmp=tmrBitCnt;
}

void confirmarFlanco (void)
{	
	// ... =_tmrFlancoPinEstadoTmp;
	// ... =_tmrFlancoTmrBitCntTmp;
	// Flag de Flanco Pendiente=0x01;
}

// CAMBIO PIN

void isrCambioPin (void)
{
	unsigned short int tmrBitCnt;	
	unsigned char pinEstado;	
	// get tmrBitVal
	// get PinEstado.	
	startTmrFlanco (pinEstado, tmrBitCnt);
}

// TMR BIT.

void isrTmrBit (void)
{
	// Marca flag que indica el estado del Pin. doTask() hará el resto.
}








*/

















void daliAddressingSystemExtensionGotoStage(unsigned char stage)
{
    _dAQueryMissingSAddrSent=NOT_SENDING;
    daliAddressingSystemExtensionStage=stage;
}





void daliAddressingSystemExtensionDoTask (void)
{
    /*
		unsigned char address;
    if (daliAddressingSystemExtensionStatus==IN_PROGRESS)
    {
        if (daliAddressingSystemExtensionStage==STAGE_0)
        {
            daliAddressingSystemExtensionGotoStage(STAGE_1);
        }
        if (daliAddressingSystemExtensionStage==STAGE_1)
        {
            if (_dAQueryMissingSAddrSent==NOT_SENDING)        dAQueryMissingSAddrSend();
            if (_dAQueryMissingSAddrSent==SENT)
            {
                if (queryMissingShortAddress==TRUE)
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_2);
                }
                else
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_11);
                }
            }
        }
        if (daliAddressingSystemExtensionStage==STAGE_2)
        {
            if (_dAQueryCGLoopSent==NOT_SENDING)            dAQueryCGLoopSend();
            if (_dAQueryCGLoopSent==SENT)
            {
                if (ocupacionDireccionesCnt<64)
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_3);
                }
                else
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_11);
                }
            }
        }
        if (daliAddressingSystemExtensionStage==STAGE_3)
        {
            if (_dATerminateSent==NOT_SENDING)        dATerminateSend();
            if (_dATerminateSent==SENT)               dAInitializeSend(0xFF);
            if (_dAInitializeSent==SENT)              dARandomizeSend();
            if (_dARandomizeSent==SENT)
            {
                daliAddressingSystemExtensionGotoStage(STAGE_4);
            }
        }
        if (daliAddressingSystemExtensionStage==STAGE_4)
        {
            if (_dASRAStatus==NOT_STARTED)           _dASRAStart(ALL_DEVICES,TRUE);
            if (_dASRAStatus==DONE)
            {
                if  (_dASRAEncontradosCnt>0)
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_5);
                }
                else
                {
                    daliAddressingSystemExtensionGotoStage(STAGE_12);
                }
            }
        }
        if (daliAddressingSystemExtensionStage==STAGE_5)
        {
            //
                                        //programAddress=daliAddressingFirstFreeAddressGet();

            // ProgramShortAddress(address);

            // programa la primera randomAddress en la primera direccion Libre.
            // marca esa dirección como ocupada.
            // repite el proceso.
        }
        if (daliAddressingSystemExtensionStage==STAGE_6)
        {
            // VerifyShortAddress(address);
        }
        if (daliAddressingSystemExtensionStage==STAGE_7)
        {
            if (_dATerminateSent==NOT_SENDING)        dATerminateSend();
            if (_dATerminateSent==SENT)
            {
                daliAddressingSystemExtensionGotoStage(STAGE_6);
            }
        }
        if (daliAddressingSystemExtensionStage==STAGE_12)
        {
            daliAddressingSystemExtensionStatus=DONE;
        }
    }
		*/
}




void daliAddressingDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)
{
	dAInitializeCb(daliSentDest);
	dARandomizeCb(daliSentDest);
	dAWithdrawCb(daliSentDest);
	dATerminateCb(daliSentDest);
	dAQueryMissingSAddrCb (daliSentCommandLow, daliReplyValid, daliReplyAnswer);
	dASearchAddrHCb (daliSentDest);
	dASearchAddrMCb (daliSentDest);
	dASearchAddrLCb (daliSentDest);
	dACompareCb (daliSentDest, daliReplyValid, daliReplyAnswer);
	dAQueryCGLoopCb (daliSentCommandLow, &daliReplyValidArray[0], &daliReplyAnswerArray[0]);
	dAQGroupsLowCb (daliSentCommandLow, &daliReplyValidArray[0], &daliReplyAnswerArray[0]);
	dAQGroupsHighCb (daliSentCommandLow, &daliReplyValidArray[0], &daliReplyAnswerArray[0]);
	dAQueryShortAddrCb (daliSentDest, daliReplyValid, daliReplyAnswer);
	dAProgramSAddrCb (daliSentDest);	
	dADTRCb(daliSentDest);
	dAStoreDTRAsAddrCb(daliSentCommandLow);		
	//dAQueryDTLoopCb(daliSentCommandLow, &daliReplyValidArray[0], &daliReplyAnswerArray[0]);
	qSNDTRCb(daliSentDest);	
	qSNDTR1Cb(daliSentDest);
	qSNReadMemoryLocationCb(daliSentCommandLow, daliReplyValid, daliReplyAnswer);	
	dAQueryDTCb(daliSentCommandLow, daliSentDest, daliReplyValid, daliReplyAnswer); 
}




























































































// >> PARA QUE NO DE ERROR.

        unsigned char daliAddressingNewInitialisationOpPending;
        unsigned char found;


				void daliAddressingLineScanStart (unsigned char *address)
        {
					_ipA[0]=address[0];
					_ipA[1]=address[1];
					_ipA[2]=address[2];
					_ipA[3]=address[3];
					_comA[0]=0x40;
					_comA[1]=0x00;
					_newInitType=0x00;
					if (dALSStart()==0x01)
					{
						daliAddressingNotify(0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF);
					}
					else
					{
						daliAddressingNotify(0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
					}
        }
				
        unsigned char daliAddressingGroupScanStart (void)
        {
                return 0x00;
        }
        
				

				
				
				
				
				
				
				void daliAddressingSystemExtensionStart (unsigned char *address)
				{
					_ipA[0]=address[0];
					_ipA[1]=address[1];
					_ipA[2]=address[2];
					_ipA[3]=address[3];
					_comA[0]=0x40;
					_comA[1]=0x09;			
					_newInitType=0x00;
					if (dASEStart()==0x01)
					{
						daliAddressingNotify(0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF);
					}
					else
					{
						daliAddressingNotify(0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
					}					
				}
				
			
				void daliAddressingNewInitialisationStart (unsigned char *address, unsigned char newInitType)
        {          
					_ipA[0]=address[0];
					_ipA[1]=address[1];
					_ipA[2]=address[2];
					_ipA[3]=address[3];
					
					if (newInitType==0x00)
					{					
						_comA[0]=0x40;
						_comA[1]=0x08;					
					}
					if (newInitType==0x01)
					{					
						_comA[0]=0x77;
						_comA[1]=0x01;					
					}	
					
					_newInitType=newInitType;
					
					if (dANIStart()==0x01)
					{
						daliAddressingNotify(0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF);
					}
					else
					{
						daliAddressingNotify(0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
					}
        }
        
				
				void daliAddressingTimeoutTick (void)
        {
        }			
				void daliAddressingDoTask (void)
        {						
					dASEDoTask();
					dANIDoTask();
					dASRADoTask();
					dAQCGLDoTask();
					dAPSADoTask();					
					dADSADoTask();	
					dALSDoTask();
					dAQDTLDoTask();
					qSNDoTask();
        }
        unsigned char daliAddressingDefaultGroupsSetStart (void)
        {
					return 0x00;
        }

// << PARA QUE NO DE ERROR.



/*

// daliAddressing.c
// dADSA    Delete short address.
// daLS     Line Scanning.
// daNI     New initialisation.
// daSE     System Extension.
// daPSA    Program Short Address.
// daQCGL   Query Control Gear [LOOP].
// daQDTL   Query Device Type [LOOP].
// daSRA    Search Random Address.


// Variables globales relacionadas con el Addressing.
// ENVIO DE COMANDOS.
    // NOT_SENDING - SENDING ó SENT.
        unsigned char   _dA_SearchAddrHSent;                        //  1
        unsigned char   _dA_SearchAddrMSent;                        //  2
        unsigned char   _dA_SearchAddrLSent;                        //  3
        unsigned char   _dA_CompareSent;                            //  4
        unsigned char   _dA_QueryCGLoopSent;                        //  5
        unsigned char 	_dA_QueryDTLoopSent;                        //  6
        unsigned char   _dA_QueryMissingSAddrSent;                  //  7
        unsigned char   _dA_InitializeSent;                         //  8
        unsigned char   _dA_WithdrawSent;                           //  9
        unsigned char   _dA_TerminateSent;                          //  10
        unsigned char   _dA_RandomizeSent;                          //  11
        unsigned char   _dA_QuerySAddrSent;                         //  12
        unsigned char	_dA_ProgramSAddrSent;                       //  13
        unsigned char	_dA_DTRSent;                                //  14
        unsigned char	_dA_StoreDTRAsAddrSent;                     //  15
    // Faltaría:    
        unsigned char   _dA_QueryGroupsHSent;                       //  16
        unsigned char   _dA_QueryGroupsLSent;                       //  17

    // Resultado envío de comandos:
        BOOL            _dA_CompareResult;                          //  18
        BOOL            _dA_QueryMissingSAddrResult;                //  19
        BOOL            _dA_QuerySAddrReplyValid;                   //  20
        unsigned char   _dA_QuerySAddrReplyAnswer;                  //  21
        // Query control gear necesita una matriz.                  // [8+64]
        // Query device type necesita una matriz.                   // [8+64]
        // Query groups H necesita una matriz.                      // [8+64]
        // Query groups L necesita una matriz.                      // [8+64]
        // Actualmente Query Device type lo guarda en su propia matriz. [64]
        // Actualmente Query Device type lo guarda en su propia matriz  [8+64]
        // Actualmente Query Groups M lo graba en flash directamente.   []
        // Actualmente Query Groups H lo graba en flash directamente.   []               
        // Para el almacenamiento de los numeros de serie se utiliza una matriz de 4*64 = 256
        // El tamaño total de almacenamiento temporal, sería de unos 600 bytes.
        
 */       
      




