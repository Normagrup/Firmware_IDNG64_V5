#include "daliCommands.h"
#include "dali.h"
#include "daliLoop.h"
#include "config.h"




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



#define COMANDO_DALI_QUERY_TYPE														0x00
#define	COMANDO_DALI_QUERY_ENABLED													0x10
#define	COMANDO_DALI_QUERY_MODE														0x11

#define	COMANDO_DALI_QUERY_SERIAL_1													0x02
#define	COMANDO_DALI_QUERY_SERIAL_2													0x03
#define	COMANDO_DALI_QUERY_SERIAL_3													0x04
#define	COMANDO_DALI_QUERY_SERIAL_4													0x05


unsigned char _querySensorType;							//1
unsigned char _querySensorEnabled;					//1
unsigned char _querySensorMode;							//1
unsigned char _querySensorSubMode;					//1
unsigned char _querySensorSerial[4];				//4
unsigned char _sensorData[8];								//8
unsigned char _sensorScanningStatus;				//1
unsigned char _sensorScanningStage;					//1
unsigned char _err;													//1		
unsigned char _num;													//1
unsigned char _cnt;													//1
unsigned char _sensorScanningIp[4];					//4
unsigned char _rei;													//1
//														---------------------------------------
																						// 26 bytes


void SendQuerySensorType (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorType==NOT_SENDING)
	{		
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_TYPE)==0x01)
		{
			_querySensorType=SENDING;
		}
	}
}

void SendQuerySensorEnabled (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorEnabled==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_ENABLED)==0x01)
		{
			_querySensorEnabled=SENDING;
		}
	}
}

void SendQuerySensorMode (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorMode==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_MODE)==0x01)
		{
			_querySensorMode=SENDING;
		}
	}
}



void SendQuerySensorSerial1 (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorSerial[0]==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_SERIAL_1)==0x01)
		{
			_querySensorSerial[0]=SENDING;
		}
	}
}

void SendQuerySensorSerial2 (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorSerial[1]==NOT_SENDING)
	{			
		sysTickDelay (10);								
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_SERIAL_2)==0x01)
		{
			_querySensorSerial[1]=SENDING;		
		}
	}
}

void SendQuerySensorSerial3 (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorSerial[2]==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_SERIAL_3)==0x01)
		{
			_querySensorSerial[2]=SENDING;
		}
	}
}

void SendQuerySensorSerial4 (unsigned char dest)
{
	unsigned char apply[8];
	if  (_querySensorSerial[3]==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_SERIAL_4)==0x01)
		{
			_querySensorSerial[3]=SENDING;
		}
	}
}








void QuerySensorTypeCb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorType==SENDING) && (command==COMANDO_DALI_QUERY_TYPE))
	{
		if (valid==0x01)
		{
			_sensorData[0]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorType=SENT;
	}	
}

void QuerySensorEnabledCb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorEnabled==SENDING) && (command==COMANDO_DALI_QUERY_ENABLED))
	{
		if (valid==0x01)
		{
			_sensorData[1]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorEnabled=SENT;		
	}	
}

void QuerySensorModeCb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorMode==SENDING) && (command==COMANDO_DALI_QUERY_MODE))
	{
		if (valid==0x01)
		{
			_sensorData[2]=answer;
		}
		else
		{
			_err=0x01;
		}							
		_querySensorMode=SENT;
	}	
}



void QuerySensorSerial1Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorSerial[0]==SENDING) && (command==COMANDO_DALI_QUERY_SERIAL_1))
	{
		if (valid==0x01)
		{
			_sensorData[4]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorSerial[0]=SENT;
	}	
}

void QuerySensorSerial2Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorSerial[1]==SENDING) && (command==COMANDO_DALI_QUERY_SERIAL_2))
	{
		if (valid==0x01)
		{
			_sensorData[5]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorSerial[1]=SENT;
	}	
}

void QuerySensorSerial3Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorSerial[2]==SENDING) && (command==COMANDO_DALI_QUERY_SERIAL_3))
	{
		if (valid==0x01)
		{
			_sensorData[6]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorSerial[2]=SENT;
	}	
}

void QuerySensorSerial4Cb (unsigned char command, unsigned char valid, unsigned char answer)
{
	if ((_querySensorSerial[3]==SENDING) && (command==COMANDO_DALI_QUERY_SERIAL_4))
	{
		if (valid==0x01)
		{
			_sensorData[7]=answer;
		}
		else
		{
			_err=0x01;
		}		
		_querySensorSerial[3]=SENT;
	}	
}










void sensorScanningGotoStage(unsigned char stage)
{
	_querySensorType=NOT_SENDING;
	_querySensorEnabled=NOT_SENDING;
	_querySensorMode=NOT_SENDING;
	_querySensorSubMode=NOT_SENDING;
	_querySensorSerial[0]=NOT_SENDING;
	_querySensorSerial[1]=NOT_SENDING;
	_querySensorSerial[2]=NOT_SENDING;
	_querySensorSerial[3]=NOT_SENDING;
	_sensorScanningStage=stage;	
	_err=0;
}

void sensorScanningNotify(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5, unsigned char byte6)
{	
	unsigned char respuesta[10];	
	
	//BYTE1 -> NO VA A INICIAR LA OPERACION / VA A INICIAR LA OPERACION O LA ESTÁ HACIENDO.
	//BYTE2 -> VA A INICIAR LA OPERACION / ESTÁ HACIENDO LA OPERACION / FINALIZÓ LA OPERACIÓN.
	//BYTE3 -> NOK / OK.
	//BYTE4 -> INFO ERROR.
	//BYTE5 -> SUBOPERACION.
	//BYTE6 -> NUMERO EQUIPOS.	
		respuesta[0]=0x83;
		respuesta[1]=0x77;
		respuesta[2]=0x02;
		respuesta[3]=0x05;		
		respuesta[4]=byte1;
		respuesta[5]=byte2;
		respuesta[6]=byte3;
		respuesta[7]=byte4;
		respuesta[8]=byte5;
		respuesta[9]=byte6;
		ethSendFrame(0,&respuesta[0],10,&_sensorScanningIp[0], 2234);			
}




void sensorScanningStart (unsigned char *ip)
{	
	unsigned char n;
	unsigned char cfg[8];	
	
	if (daliAccessHandlerAllowOnlyThisModule (DALI_SENSORS, 0x00, 0x00)==0x01)
	{	
	
	_sensorScanningIp[0]=ip[0];
	_sensorScanningIp[1]=ip[1];
	_sensorScanningIp[2]=ip[2];
	_sensorScanningIp[3]=ip[3];		
	sensorScanningNotify(0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF);	
	configInterfacesIniciarConfiguracion();
	_sensorScanningStatus=IN_PROGRESS;	
	 sensorScanningGotoStage(STAGE_0);
	_err=0x00;
	_num=16;	
	_cnt=0;		
	_rei=5;
	}
}









void sensorScanningDoTask (void)
{
	
	unsigned char cfg[8];
	unsigned char n;
	
	if (_sensorScanningStatus==IN_PROGRESS)
	{
		if (_sensorScanningStage==STAGE_0)
		{		
			if (_querySensorType==NOT_SENDING)	SendQuerySensorType(((_cnt<<1)+1));
			if (_querySensorType==SENT)					SendQuerySensorEnabled(((_cnt<<1)+1));
			if (_querySensorEnabled==SENT)			SendQuerySensorMode(((_cnt<<1)+1));
			if (_querySensorMode==SENT)					SendQuerySensorSerial1(((_cnt<<1)+1));
			if (_querySensorSerial[0]==SENT)		SendQuerySensorSerial2(((_cnt<<1)+1));
			if (_querySensorSerial[1]==SENT)		SendQuerySensorSerial3(((_cnt<<1)+1));
			if (_querySensorSerial[2]==SENT)		SendQuerySensorSerial4(((_cnt<<1)+1));
			if (_querySensorSerial[3]==SENT)
			{
					if ((_cnt<_num) && (_rei>0))
					{				
						if (_err==0x00)
						{
							// NO HUBO ERRORES EN EL ÚLTIMO GUARDADO.												
								cfg[0]=0x01;	//Instalado.					
								cfg[1]=_sensorData[1];	// Hay que invertirlo.				
								cfg[2]=_sensorData[0];	//Tipo
								cfg[3]=0x00;					
								if (_sensorData[2]==0x08)		cfg[3]=1;//INTERFACES_MODO_PULSADOR_FLANCO_ASCENDENTE;
								if (_sensorData[2]==0x10)		cfg[3]=2;//INTERFACES_MODO_PULSADOR_FLANCO_DESCENDENTE;
								if (_sensorData[2]==0x18)		cfg[3]=3;//INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG;
								if (_sensorData[2]==0x20)		cfg[3]=4;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO;
								if (_sensorData[2]==0x28)		cfg[3]=5;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL;
								if (_sensorData[2]==0x30)		cfg[3]=6;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ;
								if (_sensorData[2]==0x38)		cfg[3]=7;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ;
								if (_sensorData[2]==0x40)		cfg[3]=8;//INTERFACES_MODO_INTERFAZ_REG_LUZ;				
								if (_sensorData[2]==0x48)		cfg[3]=9;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ_INT;
								if (_sensorData[2]==0x50)		cfg[3]=10;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ_INT;
								if (_sensorData[2]==0x58)		cfg[3]=11;//INTERFACES_MODO_INTERFAZ_REG_LUZ_INT;										
								cfg[4]=_sensorData[4];	//Serial1
								cfg[5]=_sensorData[5];	//Serial1
								cfg[6]=_sensorData[6];	//Serial1
								cfg[7]=_sensorData[7];	//Serial1	
								_rei=5;
								configInterfacesConfiguracionSet(_cnt,&cfg[0]);					
								_cnt++;				
								sensorScanningGotoStage(STAGE_0);													
						}
						else
						{
							// HUBO UN ERROR.
							if (_rei>0)
							{
								// MENOS DE 5 REINTENTOS -> REINTENTAMOS.
								_rei--;
								sensorScanningGotoStage(STAGE_0);							
							}			
						}						
					}
					else
					{
						cfg[0]=0x00;
						cfg[1]=0xFF;
						cfg[2]=0xFF;
						cfg[3]=0xFF;					
						cfg[4]=0xFF;
						cfg[5]=0xFF;
						cfg[6]=0xFF;
						cfg[7]=0xFF;		
						for (n=_cnt;n<16;n++)
						{
							configInterfacesConfiguracionSet(n,&cfg[0]);
						}
						configInterfacesFinalizarConfiguracion();
						sensorScanningGotoStage(STAGE_1);
						_sensorScanningStatus=DONE;			
						sensorScanningNotify(0x01,0x02,0x01,0xFF,0xFF,_cnt);								
						interfacesInit();	
						daliAccessHandlerAllowAllModules();								
					}				
				}	
			}
		}		
}