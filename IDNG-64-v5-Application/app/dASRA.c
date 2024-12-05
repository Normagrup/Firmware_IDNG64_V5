		#include "dali.h"
				
		#define DASRA_GLOBAL_PROCESS_TIMEOUT		600
		#define DASRA_DEVICE_PROCESS_TIMEOUT		30
		#define NOK         				0x00
		#define OK         					0x01
		#define NA         					0xFF		
		#define ERR_NE       				0x00
		#define ERR_TOO_MUCH				0x01		
		#define ERR_TIMEOUT					0x02			
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
	
		void dAQueryShortAddrSend (void);
		void dAInitializeSend (unsigned char parameter);
		void dARandomizeSend (void);
		void dAWithdrawSend (void);
		void dATerminateSend (void);
		void dAQueryMissingSAddrSend (void);
		void dASearchAddrHSend (unsigned char value);
		void dASearchAddrMSend (unsigned char value);
		void dASearchAddrLSend (unsigned char value);
		void dACompareSend (void);
		void dAQueryCGLoopSend (void);

		// External.
    extern unsigned char    _dASearchAddrHSent;
    extern unsigned char    _dASearchAddrMSent;
    extern unsigned char    _dASearchAddrLSent;
    extern unsigned char    _dACompareSent;
    extern BOOL             _dACompareResult;
    extern unsigned char    _dAQueryCGLoopSent;
    extern unsigned char    _dAQueryMissingSAddrSent;
    extern BOOL             _dAQueryMissingSAddrResult;
    extern unsigned char    _dAInitializeSent;
    extern unsigned char    _dAWithdrawSent;
    extern unsigned char    _dATerminateSent;
    extern unsigned char    _dARandomizeSent;
    extern unsigned char    _dAQuerySAddrSent;
    extern BOOL             _dAQuerySAddrReplyValid;
    extern unsigned char    _dAQuerySAddrReplyAnswer;
		// Internal.
		unsigned char 					_dASRAReintentosCnt;		
		unsigned char   				_dASRAStage;																										// Varible utilizada para el algoritmo de búsqueda.
    BOOL            				_dASRAFound;    																								// Varible utilizada para el algoritmo de búsqueda.
    BOOL            				_dASRARandomizeIsNeeded;																				// Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAInitializeMode;																						// Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRABusqueda;                                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAContador;                                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchH;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchH;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchM;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchM;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloSuperiorSearchL;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAIntervaloInferiorSearchL;                                 // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchH;                                           // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchM;                                           // Varible utilizada para el algoritmo de búsqueda.
    unsigned char   				_dASRAHalladoSearchL;                                           // Varible utilizada para el algoritmo de búsqueda.		
		unsigned short int			_dASRAGlobalProcTimeout;
		unsigned char						_dASRADeviceProcTimeout;    
		unsigned char   				_dASRAStatus;																										// Salida
    unsigned char   				_dASRAResult;																										// Salida	
		unsigned char   				_dASRAExitCode;																									// Salida	
		unsigned char 					_dASRAEncontradosCnt;                                           // Salida
    unsigned char 					_dASRAEncontradosList[4][64];                                   // Salida		
		
		
		unsigned char dASRAEncontradosHGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[0][equipo]);
		}
		unsigned char dASRAEncontradosMGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[1][equipo]);
		}
		unsigned char dASRAEncontradosLGet(unsigned char equipo)
		{
			return (_dASRAEncontradosList[2][equipo]);
		}
		
		
		void dASRARtcTick (void)
		{
			if (_dASRAStatus==IN_PROGRESS)
			{
				if (_dASRAGlobalProcTimeout>0)
				{
					_dASRAGlobalProcTimeout--;
				}
				else
				{
					daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_SEARCH_RANDOM_ADDRESS);
					_dASRAResult=NOK;
					_dASRAExitCode=ERR_TIMEOUT;											     
					_dASRAStatus=DONE;
				}
				
				if (_dASRADeviceProcTimeout>0)
				{
					_dASRADeviceProcTimeout--;
				}
				else
				{
					daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_SEARCH_RANDOM_ADDRESS);
					_dASRAResult=NOK;
					_dASRAExitCode=ERR_TIMEOUT;											     
					_dASRAStatus=DONE;
				}				
			}			
		}
		
		
		static void dASRAGotoStage (unsigned char stage)
    {
        _dAInitializeSent=NOT_SENDING;
        _dARandomizeSent=NOT_SENDING;
        _dATerminateSent=NOT_SENDING;
        _dASearchAddrHSent=NOT_SENDING;
        _dASearchAddrMSent=NOT_SENDING;
        _dASearchAddrLSent=NOT_SENDING;
        _dACompareSent=NOT_SENDING;
        _dAWithdrawSent=NOT_SENDING;
        _dAQuerySAddrSent=NOT_SENDING;
        _dASRAStage=stage;
    }
    
		BOOL dASRAStart (unsigned char initializeMode, BOOL randomizeIsNeeded)
    {
				// también debería devolverte que NO en el caso de que el transceptor esté bloqueado para otra cosa.
				BOOL retVal;
        unsigned char n;
        if (_dASRAStatus!=IN_PROGRESS) 
        {
						if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_SEARCH_RANDOM_ADDRESS)==0x01)
						{					
							_dASRAResult=0x00;
							_dASRAReintentosCnt=3;
							_dASRAExitCode=0xFF;
							_dASRAInitializeMode=initializeMode;
							_dASRARandomizeIsNeeded=randomizeIsNeeded;							
							_dASRAGlobalProcTimeout=DASRA_GLOBAL_PROCESS_TIMEOUT;
							_dASRADeviceProcTimeout=DASRA_DEVICE_PROCESS_TIMEOUT;				
							_dASRAEncontradosCnt=0;
							for (n=0;n<64;n++)
							{
									_dASRAEncontradosList[0][n]=0xFF;
									_dASRAEncontradosList[1][n]=0xFF;
									_dASRAEncontradosList[2][n]=0xFF;
									_dASRAEncontradosList[3][n]=0xFF;
							}
							dASRAGotoStage(STAGE_0);
							_dASRAStatus=IN_PROGRESS;
							retVal=TRUE;
						}
						else
						{
							retVal=FALSE;
							_dASRAStatus=DONE;
						}
        }
        else
        {
					retVal=FALSE;
					_dASRAStatus=DONE;
        }
        return retVal;
    }
    
		
		
		
		
		
		
		
		void dASRAStop (void)
		{
				if (_dASRAStatus==IN_PROGRESS)
				{
					dASRAGotoStage(STAGE_14);				
				}
		}
		
		
		
		void dASRADoneFlagClear (void)
		{
			_dASRAStatus=NOT_STARTED;
		}
		
		
		
		void dASRADoTask (void)
    {
        unsigned char h;
				unsigned char m;
				unsigned char l;			
			
				if (_dASRAStatus==IN_PROGRESS)
        {
            if (_dASRAStage==STAGE_0)
            {
                _dASRAFound=FALSE;
								daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x00,0);
                dASRAGotoStage(STAGE_1);
            }
            if (_dASRAStage==STAGE_1)
            {    						
								if (_dATerminateSent==NOT_SENDING)       					dATerminateSend();
                if (_dATerminateSent==SENT)              					dAInitializeSend(_dASRAInitializeMode);
                if (_dAInitializeSent==SENT)
                {
                    if (_dASRARandomizeIsNeeded==TRUE)
                    {
                        dARandomizeSend();
                        if (_dARandomizeSent==SENT)
                        {
                            dASRAGotoStage(STAGE_2);
                        }
                    }
                    else
                    {
                        dASRAGotoStage(STAGE_2);
                    }
                }
            }
            if (_dASRAStage==STAGE_2)
            {
                // Envía SEARCH ADDRESS con el FF. FF. FF .
                // Después hace un COMPARE.
                // El resultado del COMPARE debe ser TRUE. De lo contrario significa que no hay equipos.
                if (_dASearchAddrHSent==NOT_SENDING)           dASearchAddrHSend(0xFF);
                if (_dASearchAddrHSent==SENT)                  dASearchAddrMSend(0xFF);
                if (_dASearchAddrMSent==SENT)                   dASearchAddrLSend(0xFF);
                if (_dASearchAddrLSent==SENT)                   dACompareSend();
                if (_dACompareSent==SENT)
                {									
										if (_dACompareResult==TRUE)
                    {
                        // Hay equipos.
                        dASRAGotoStage(STAGE_3);
                    }
                    else
                    {
                        // No hay equipos.
                        dASRAGotoStage(STAGE_14);
												daliAddressingDebug(0x01,0x02,0xFF,0x93,0xFF,0xFF);
                    }
                }
            }
            if (_dASRAStage==STAGE_3)
            {
                // Inicializacion _dASRABusqueda SEARCH H.
                _dASRAIntervaloInferiorSearchH=0x00;
                _dASRAIntervaloSuperiorSearchH=0xFF;
                _dASRAContador=8;
                _dASRABusqueda=128;
                if (_dASearchAddrLSent==NOT_SENDING)            dASearchAddrLSend(0x00);
                if (_dASearchAddrLSent==SENT)                   dASearchAddrMSend(0x00);
                if (_dASearchAddrMSent==SENT)
                {
                    dASRAGotoStage(STAGE_4);
                }
            }
            if (_dASRAStage==STAGE_4)
            {
                // _dASRABusqueda SEARCH H.
                if (_dASearchAddrHSent==NOT_SENDING)           dASearchAddrHSend(_dASRABusqueda);
                if (_dASearchAddrHSent==SENT)                  dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAIntervaloSuperiorSearchH=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda-(0x01<<(_dASRAContador-2));
                    }
                    else
                    {
                        _dASRAIntervaloInferiorSearchH=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda+(0x01<<(_dASRAContador-2));
                    }
                    _dASRAContador--;
                    if (_dASRAContador>0)
                    {
                        dASRAGotoStage(STAGE_4);
                    }
                    else
                    {
                        dASRAGotoStage(STAGE_5);
                    }
                }
            }
            if (_dASRAStage==STAGE_5)
            {
                // Hallado SEARCH H.
                if (_dASearchAddrHSent==NOT_SENDING)		dASearchAddrHSend(_dASRAIntervaloInferiorSearchH);
                if (_dASearchAddrHSent==SENT)					dASearchAddrLSend(0xFF);
                if (_dASearchAddrLSent==SENT)						dASearchAddrMSend(0xFF);
                if (_dASearchAddrMSent==SENT)						dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAHalladoSearchH=_dASRAIntervaloInferiorSearchH;
                    }
                    else
                    {
                        _dASRAHalladoSearchH=_dASRAIntervaloInferiorSearchH+1;
                    }
                    dASRAGotoStage(STAGE_6);
                }
            }
            if (_dASRAStage==STAGE_6)
            {
                // Inicializacion _dASRABusqueda SEARCH M.
                _dASRAIntervaloInferiorSearchM=0x00;
                _dASRAIntervaloSuperiorSearchM=0xFF;
                _dASRAContador=8;
                _dASRABusqueda=128;
                if (_dASearchAddrHSent==NOT_SENDING)		dASearchAddrHSend(_dASRAHalladoSearchH);
                if (_dASearchAddrHSent==SENT)					dASearchAddrLSend(0);
                if (_dASearchAddrLSent==SENT)
                {
                    dASRAGotoStage(STAGE_7);
                }
            }
            if (_dASRAStage==STAGE_7)
            {
                // _dASRABusqueda SEARCH M.
                if (_dASearchAddrMSent==NOT_SENDING)		dASearchAddrMSend(_dASRABusqueda);
                if (_dASearchAddrMSent==SENT)			dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAIntervaloSuperiorSearchM=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda-(0x01<<(_dASRAContador-2));
                    }
                    else
                    {
                        _dASRAIntervaloInferiorSearchM=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda+(0x01<<(_dASRAContador-2));
                    }
                    _dASRAContador--;
                    if (_dASRAContador>0)
                    {
                        dASRAGotoStage(STAGE_7);
                    }
                    else
                    {
                        dASRAGotoStage(STAGE_8);
                    }
                }
            }
            if (_dASRAStage==STAGE_8)
            {
                // Hallado SEARCH M.
                if (_dASearchAddrMSent==NOT_SENDING)		dASearchAddrMSend(_dASRAIntervaloInferiorSearchM);
                if (_dASearchAddrMSent==SENT)			dASearchAddrLSend(0xFF);
                if (_dASearchAddrLSent==SENT)			dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAHalladoSearchM=_dASRAIntervaloInferiorSearchM;
                    }
                    else
                    {
                        _dASRAHalladoSearchM=_dASRAIntervaloInferiorSearchM+1;
                    }
                    dASRAGotoStage(STAGE_9);
                }
            }
            if (_dASRAStage==STAGE_9)
            {
                // Inicializacion _dASRABusqueda SEARCH L.
                _dASRAIntervaloInferiorSearchL=0x00;
                _dASRAIntervaloSuperiorSearchL=0xFF;
                _dASRAContador=8;
                _dASRABusqueda=128;
                if (_dASearchAddrHSent==NOT_SENDING)           dASearchAddrHSend(_dASRAHalladoSearchH);
                if (_dASearchAddrHSent==SENT)                  dASearchAddrMSend(_dASRAHalladoSearchM);
                if (_dASearchAddrMSent==SENT)
                {
                    dASRAGotoStage(STAGE_10);
                }
            }
            if (_dASRAStage==STAGE_10)
            {
                // _dASRABusqueda SEARCH L.
                if (_dASearchAddrLSent==NOT_SENDING)           dASearchAddrLSend(_dASRABusqueda);
                if (_dASearchAddrLSent==SENT)                  dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAIntervaloSuperiorSearchL=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda-(0x01<<(_dASRAContador-2));
                    }
                    else
                    {
                        _dASRAIntervaloInferiorSearchL=_dASRABusqueda;
                        _dASRABusqueda=_dASRABusqueda+(0x01<<(_dASRAContador-2));
                    }
                    _dASRAContador--;
                    if (_dASRAContador>0)
                    {
                        dASRAGotoStage(STAGE_10);
                    }
                    else
                    {
                        dASRAGotoStage(STAGE_11);
                    }
                }
            }
            if (_dASRAStage==STAGE_11)
            {
                // Hallado SEARCH L.
                if (_dASearchAddrLSent==NOT_SENDING)              dASearchAddrLSend(_dASRAIntervaloInferiorSearchL);
                if (_dASearchAddrLSent==SENT)                     dACompareSend();
                if (_dACompareSent==SENT)
                {
                    if (_dACompareResult==TRUE)
                    {
                        _dASRAHalladoSearchL=_dASRAIntervaloInferiorSearchL;
                    }
                    else
                    {
                        _dASRAHalladoSearchL=_dASRAIntervaloInferiorSearchL+1;
                    }
                    _dASRAFound=TRUE;
                    dASRAGotoStage(STAGE_12);
                }
            }
            if (_dASRAStage==STAGE_12)
            {
                if (_dASearchAddrHSent==NOT_SENDING)						dASearchAddrHSend(_dASRAHalladoSearchH);
                if (_dASearchAddrHSent==SENT)										dASearchAddrMSend(_dASRAHalladoSearchM);
                if (_dASearchAddrMSent==SENT)										dASearchAddrLSend(_dASRAHalladoSearchL);
                if (_dASearchAddrLSent==SENT)                   dAQueryShortAddrSend();
                if (_dAQuerySAddrSent==SENT)
                {
                    dASRAGotoStage(STAGE_13);
                }
            }
            if (_dASRAStage==STAGE_13)
            {
                if (_dAWithdrawSent==NOT_SENDING)						dAWithdrawSend();
                if (_dAWithdrawSent==SENT)
                {
                    dASRAGotoStage(STAGE_14);
                }
            }
            if (_dASRAStage==STAGE_14)
            {
                if (_dASRAFound==TRUE)
                {
                    // >> RANDOM ADDRESS STORE.
                    if (_dASRAEncontradosCnt<64)
                    {

                        if (_dAQuerySAddrReplyValid==TRUE)
                        {												
														_dASRAEncontradosList[0][_dASRAEncontradosCnt]=_dASRAHalladoSearchH;
														_dASRAEncontradosList[1][_dASRAEncontradosCnt]=_dASRAHalladoSearchM;
														_dASRAEncontradosList[2][_dASRAEncontradosCnt]=_dASRAHalladoSearchL;
														h=_dASRAEncontradosList[0][_dASRAEncontradosCnt];
														m=_dASRAEncontradosList[1][_dASRAEncontradosCnt];
														l=_dASRAEncontradosList[2][_dASRAEncontradosCnt];
													
														if (_dAQuerySAddrReplyAnswer!=0xFF)
                            {
                                _dASRAEncontradosList[3][_dASRAEncontradosCnt]=(_dAQuerySAddrReplyAnswer>>1);
                            }
                            else
                            {
                                _dASRAEncontradosList[3][_dASRAEncontradosCnt]=0xFF;
                            }
                            _dASRAEncontradosCnt++;
														_dASRADeviceProcTimeout=DASRA_DEVICE_PROCESS_TIMEOUT;
                            _dASRAFound=FALSE;			
														_dASRAReintentosCnt=3;														
														daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x00,_dASRAEncontradosCnt);
														daliAddressingDebug(0x66,_dASRAEncontradosCnt,h,m,l,_dAQuerySAddrReplyAnswer);														
                            dASRAGotoStage(STAGE_2);
                        }
                        else
                        {
                            if (_dASRAReintentosCnt>0)
														{
															_dASRAReintentosCnt--;
															daliAddressingDebug(0x44,_dASRAEncontradosCnt,_dASRAHalladoSearchH,_dASRAHalladoSearchM,_dASRAHalladoSearchL);
															dASRAGotoStage(STAGE_2);
															
														}
														else
														{
															_dASRAResult=NOK;
															_dASRAExitCode=ERR_NE;													
															dASRAGotoStage(STAGE_15);	
														}														
                        }
                    }
                    else
                    {
                      daliAddressingDebug(0x55,0xFF,0xFF,0xFF,0xFF);
											_dASRAResult=NOK;
											_dASRAExitCode=ERR_TOO_MUCH;											
											_dASRAEncontradosCnt=65;											
											dASRAGotoStage(STAGE_15);											
                    }
                    // << RANDOM ADDRESS STORE.
                }
                else
                {
                    _dASRAResult=OK;
										_dASRAExitCode=NA;											
                    dASRAGotoStage(STAGE_15);
                }
            }
            if (_dASRAStage==STAGE_15)
            {
                if (_dATerminateSent==NOT_SENDING)                   dATerminateSend();
                if (_dATerminateSent==SENT)
                {
										daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_SEARCH_RANDOM_ADDRESS);
										_dASRAStatus=DONE;
                }
            }
        }
    }


