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



#define	COMANDO_REBOOT																												0x63
#define	COMANDO_START_CONFIG																									0x16
#define	COMANDO_FINISH_CONFIG																									0x17
#define	COMANDO_DALI_ENABLE																										0x20
#define	COMANDO_DALI_DISABLE																									0x21
#define	COMANDO_DALI_SET_MODE_NO_OPERATIVO																		0x30	
#define	COMANDO_DALI_SET_MODE_PULSADOR_SHORT_FLANCO_ASCENDENTE								0x31		//1
#define	COMANDO_DALI_SET_MODE_PULSADOR_SHORT_FLANCO_DESCENDENTE								0x32		//2
#define	COMANDO_DALI_SET_MODE_PULSADOR_SHORT_LONG															0x33		//3
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_FLANCO												0x34		//4
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_NIVEL												0x35		//5
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_FLANCO_REG_LUZ								0x36		//6
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_NIVEL_REG_LUZ								0x37		//7	
#define	COMANDO_DALI_SET_MODE_REG_LUZ																					0x38		//8
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_FLANCO_REG_LUZ_INTERNO				0x39		//9
#define	COMANDO_DALI_SET_MODE_DETECTOR_PRESENCIA_NIVEL_REG_LUZ_INTERNO				0x3A		//10
#define	COMANDO_DALI_SET_MODE_REG_LUZ_INTERNO																	0x3B		//11
#define	COMANDO_DALI_QUERY_ENABLED																						0x10
#define	COMANDO_DALI_QUERY_MODE																								0x11


unsigned char _cmd_START_CONFIG;
//unsigned char _cmd_REBOOT;
unsigned char _cmd_FINISH_CONFIG;
unsigned char _cmd_ENABLE_DISABLE;
unsigned char _cmd_SET_MODE;


unsigned char 	_cmd_QUERY_MODE;
unsigned char 	_cmd_QUERY_MODE_valid;
unsigned char 	_cmd_QUERY_MODE_answer;

unsigned char 	_cmd_QUERY_ENABLE_DISABLE;
unsigned char 	_cmd_QUERY_ENABLE_DISABLE_valid;
unsigned char 	_cmd_QUERY_ENABLE_DISABLE_answer;






unsigned char	_cmd_DTR;
unsigned char _cmd_SET_SETPOINT;
unsigned char _cmd_QUERY_SETPOINT;
unsigned char _cmd_QUERY_SETPOINT_valid;
unsigned char _cmd_QUERY_SETPOINT_answer;



unsigned char _sensorStoringStatus;
unsigned char sensorStoringAddress;
unsigned char sensorStoringEnabled;
unsigned char sensorStoringMode;

unsigned char _sensorStoringSetSetpoint;
unsigned char _sensorStoringSetSetpointLevel;

unsigned char _sensorStoringQuerySetpoint;
unsigned char _sensorStoringQuerySetpointAddress;
unsigned char _sensorStoringQuerySetpointIp[4];



unsigned char _sensorStoringIp[4];
unsigned char _sensorStoringPacketId[2];
unsigned char _sensorStoringCfg[14];





unsigned char sensorStoringStart (unsigned char address, unsigned char disabled, unsigned char mode, const unsigned char *interfacesCfgIp, unsigned char packetId_1, unsigned char packetId_2, unsigned char *cfg)
{
	unsigned char answer=0;		
	unsigned char n;
	if (_sensorStoringStatus!=IN_PROGRESS)
	{			
		configInterfacesIniciarConfiguracion();
		sensorStoringAddress=address;
		sensorStoringEnabled=disabled;		
		sensorStoringMode=mode;				
		for (n=0;n<4;n++)												_sensorStoringIp[n]=interfacesCfgIp[n];
		for (n=0;n<14;n++)											_sensorStoringCfg[n]=cfg[n];
		_sensorStoringPacketId[0]=packetId_1;
		_sensorStoringPacketId[1]=packetId_2;		
		_cmd_START_CONFIG=NOT_SENDING;
		_cmd_FINISH_CONFIG=NOT_SENDING;
		_cmd_ENABLE_DISABLE=NOT_SENDING;
		_cmd_SET_MODE=NOT_SENDING;		
		_cmd_QUERY_MODE=NOT_SENDING;
		_cmd_QUERY_MODE_valid=0;
		_cmd_QUERY_ENABLE_DISABLE=NOT_SENDING;
		_cmd_QUERY_ENABLE_DISABLE_valid=0;
		_sensorStoringStatus=IN_PROGRESS;
		answer=0x01;
	}
	else
	{
		answer=0x00;
		_sensorStoringStatus=DONE;
	}
	return answer;
}

void sensorStoringQuerySetpoint (unsigned char address, unsigned char* ip)
{	
	//if (_sensorStoringQuerySetpoint!=IN_PROGRESS)
	//{			
		_sensorStoringQuerySetpointAddress=address;	
		_cmd_QUERY_SETPOINT=NOT_SENDING;
		_cmd_QUERY_SETPOINT_valid=0;	
		_sensorStoringQuerySetpointIp[0]=ip[0];
		_sensorStoringQuerySetpointIp[1]=ip[1];
		_sensorStoringQuerySetpointIp[2]=ip[2];
		_sensorStoringQuerySetpointIp[3]=ip[3];
		_sensorStoringQuerySetpoint=IN_PROGRESS;
	//}
}


unsigned char sensorStoringSetSetpoint (unsigned char address, unsigned char level)
{	
	unsigned char answer;
	if (_sensorStoringSetSetpoint!=IN_PROGRESS)	
	{
		sensorStoringAddress=address;		
		_sensorStoringSetSetpointLevel=level;		
		_cmd_START_CONFIG=NOT_SENDING;
		_cmd_FINISH_CONFIG=NOT_SENDING;		
		_cmd_DTR=NOT_SENDING;
		_cmd_SET_SETPOINT=NOT_SENDING;
	
		_sensorStoringSetSetpoint=IN_PROGRESS;
		answer=0x01;
	}		
	else
	{
		answer=0x00;
		_sensorStoringSetSetpoint=DONE;
	}
}


//void send_cmd_REBOOT (unsigned char dest)
//{
//	unsigned char apply[8];
//	if (_cmd_START_CONFIG==NOT_SENDING)
//	{
//		sysTickDelay (10);
//		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_REBOOT)==0x01)
//		{	
//			_cmd_REBOOT;
//		}
//	}	
//}



















void send_cmd_QUERY_MODE (unsigned char dest)
{
	unsigned char apply[8];
	if (_cmd_QUERY_MODE==NOT_SENDING)
	{
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_MODE)==0x01)
		{	
			_cmd_QUERY_MODE=SENDING;
		}
	}	
}

void send_cmd_QUERY_ENABLE_DISABLE (unsigned char dest)
{
	unsigned char apply[8];
	if (_cmd_QUERY_ENABLE_DISABLE==NOT_SENDING)
	{
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_DALI_QUERY_ENABLED)==0x01)
		{	
			_cmd_QUERY_ENABLE_DISABLE=SENDING;
		}
	}	
}




void send_cmd_START_CONFIG (unsigned char dest)
{
	unsigned char apply[8];
	if (_cmd_START_CONFIG==NOT_SENDING)
	{
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_START_CONFIG)==0x01)
		{	
			_cmd_START_CONFIG=SENDING;
		}
	}	
}
void send_cmd_FINISH_CONFIG (unsigned char dest)
{
	unsigned char apply[8];
	if (_cmd_FINISH_CONFIG==NOT_SENDING)
	{
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, COMANDO_FINISH_CONFIG)==0x01)
		{
			_cmd_FINISH_CONFIG=SENDING;
		}
	}	
}

void send_cmd_ENABLE_DISABLE (unsigned char dest, unsigned char arg)
{
	unsigned char apply[8];
	if ((_cmd_ENABLE_DISABLE==NOT_SENDING) && (arg<2))
	{	
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, (COMANDO_DALI_ENABLE+arg))==0x01)
		{
			_cmd_ENABLE_DISABLE=SENDING;
		}		
	}	
}


void send_cmd_SET_MODE (unsigned char dest, unsigned char arg)
{
	unsigned char apply[8];
	if ((_cmd_SET_MODE==NOT_SENDING) && (arg<12))
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, (COMANDO_DALI_SET_MODE_NO_OPERATIVO+arg))==0x01)
		{		
			_cmd_SET_MODE=SENDING;
		}
	}	
}
//void cmd_REBOOT_cb (unsigned char com)
//{
//	unsigned char apply[8];
//	if ((_cmd_REBOOT==SENDING) && (com==COMANDO_REBOOT))
//	{
//			_cmd_REBOOT=SENT;
//	}	
//}

void cmd_START_CONFIG_cb (unsigned char com)
{
	unsigned char apply[8];
	if ((_cmd_START_CONFIG==SENDING) && (com==COMANDO_START_CONFIG))
	{
			_cmd_START_CONFIG=SENT;
	}	
}
void cmd_FINISH_CONFIG_cb (unsigned char com)
{
	unsigned char apply[8];
	if ((_cmd_FINISH_CONFIG==SENDING) && (com==COMANDO_FINISH_CONFIG))
	{
			_cmd_FINISH_CONFIG=SENT;
	}	
}

void cmd_ENABLE_DISABLE_cb (unsigned char com)
{
	unsigned char apply[8];
	if ((_cmd_ENABLE_DISABLE==SENDING) && ((com==COMANDO_DALI_ENABLE) || (com==COMANDO_DALI_DISABLE)))
	{	
			_cmd_ENABLE_DISABLE=SENT;
	}	
}


void cmd_SET_MODE_cb (unsigned char com)
{
	unsigned char apply[8];
	if ((_cmd_SET_MODE==SENDING) && (com>=COMANDO_DALI_SET_MODE_NO_OPERATIVO) && (com<=COMANDO_DALI_SET_MODE_REG_LUZ_INTERNO))
	{			
			_cmd_SET_MODE=SENT;
	}	
}

void send_cmd_DTR (unsigned char dtr)
{	
	unsigned char apply[8];
	if (_cmd_DTR==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, 0xA3, &apply[0], 0x2E, dtr)==0x01)
		{		
			_cmd_DTR=SENDING;
		}
	}	
}

void cmd_DTR_cb (unsigned char dest)
{		
	if ((_cmd_DTR==SENDING) && (dest==0xA3))
	{			
			_cmd_DTR=SENT;
	}	
}

void send_cmd_SET_SETPOINT (unsigned char dest)
{
	unsigned char apply[8];
	if (_cmd_SET_SETPOINT==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, 0x22)==0x01)
		{		
			_cmd_SET_SETPOINT=SENDING;
		}
	}	
}

void cmd_SET_SETPOINT_cb (unsigned char com)
{	
	if ((_cmd_SET_SETPOINT==SENDING) && (com==0x22))
	{			
			_cmd_SET_SETPOINT=SENT;
	}	
}

void send_cmd_QUERY_SETPOINT (unsigned char dest)
{	
	unsigned char apply[8];
	if (_cmd_QUERY_SETPOINT==NOT_SENDING)
	{			
		sysTickDelay (10);
		if (daliSend(DALI_SENSORS, 0xFFFF, 0x16, dest, &apply[0], 0x2E, 0x12)==0x01)
		{		
			_cmd_QUERY_SETPOINT=SENDING;
		}
	}	
}

void cmd_QUERY_SETPOINT_cb (unsigned char com, unsigned char valid, unsigned char answer)
{
	if ((_cmd_QUERY_SETPOINT==SENDING) && (com==0x12))
	{			
			_cmd_QUERY_SETPOINT=SENT;
			_cmd_QUERY_SETPOINT_valid=valid;
			_cmd_QUERY_SETPOINT_answer=answer;
	}	
	
}

void cmd_QUERY_ENABLE_DISABLE_cb (unsigned char command, unsigned char valid, unsigned char answer)
{	
	if ((_cmd_QUERY_ENABLE_DISABLE==SENDING) && (command==COMANDO_DALI_QUERY_ENABLED))
	{
		_cmd_QUERY_ENABLE_DISABLE=SENT;
		_cmd_QUERY_ENABLE_DISABLE_valid=valid;
		_cmd_QUERY_ENABLE_DISABLE_answer=answer;
	}
}

void cmd_QUERY_MODE_cb (unsigned char command, unsigned char valid, unsigned char answer)
{	
	if ((_cmd_QUERY_MODE==SENDING) && (command==COMANDO_DALI_QUERY_MODE))
	{
		_cmd_QUERY_MODE=SENT;
		_cmd_QUERY_MODE_valid=valid;
		_cmd_QUERY_MODE_answer=0xFF;		
		if (answer==0x08)		_cmd_QUERY_MODE_answer=1;//INTERFACES_MODO_PULSADOR_FLANCO_ASCENDENTE;
		if (answer==0x10)		_cmd_QUERY_MODE_answer=2;//INTERFACES_MODO_PULSADOR_FLANCO_DESCENDENTE;
		if (answer==0x18)		_cmd_QUERY_MODE_answer=3;//INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG;
		if (answer==0x20)		_cmd_QUERY_MODE_answer=4;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO;
		if (answer==0x28)		_cmd_QUERY_MODE_answer=5;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL;
		if (answer==0x30)		_cmd_QUERY_MODE_answer=6;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ;
		if (answer==0x38)		_cmd_QUERY_MODE_answer=7;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ;
		if (answer==0x40)		_cmd_QUERY_MODE_answer=8;//INTERFACES_MODO_INTERFAZ_REG_LUZ;				
		if (answer==0x48)		_cmd_QUERY_MODE_answer=9;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ_INT;
		if (answer==0x50)		_cmd_QUERY_MODE_answer=10;//INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ_INT;
		if (answer==0x58)		_cmd_QUERY_MODE_answer=11;//INTERFACES_MODO_INTERFAZ_REG_LUZ_INT;						
	}
}







void sensorStoringDoTask (void)
{	
	unsigned char ethFrame[6];
	
	if (_sensorStoringStatus==IN_PROGRESS)
	{
		if (_cmd_START_CONFIG==NOT_SENDING)			send_cmd_START_CONFIG(((sensorStoringAddress<<1)+1));
		if (_cmd_START_CONFIG==SENT)						send_cmd_ENABLE_DISABLE(((sensorStoringAddress<<1)+1),sensorStoringEnabled);
		if (_cmd_ENABLE_DISABLE==SENT)					send_cmd_SET_MODE(((sensorStoringAddress<<1)+1),sensorStoringMode);
		if (_cmd_SET_MODE==SENT)								send_cmd_QUERY_MODE(((sensorStoringAddress<<1)+1));						
		if (_cmd_QUERY_MODE==SENT)							send_cmd_QUERY_ENABLE_DISABLE(((sensorStoringAddress<<1)+1));									
		if (_cmd_QUERY_ENABLE_DISABLE==SENT)		send_cmd_FINISH_CONFIG(((sensorStoringAddress<<1)+1));					
		if (_cmd_FINISH_CONFIG==SENT)
		{			
			ethFrame[0]=0x83;																					//	FrameType
			ethFrame[1]=_sensorStoringPacketId[0];
			ethFrame[2]=_sensorStoringPacketId[1];
			ethFrame[3]=0x01;					
			ethFrame[4]=sensorStoringAddress;		
			if (((_cmd_QUERY_ENABLE_DISABLE_valid==1) && (_cmd_QUERY_ENABLE_DISABLE_answer==sensorStoringEnabled)) &&	((_cmd_QUERY_MODE_valid==1) &&	(_cmd_QUERY_MODE_answer==sensorStoringMode)))
			{

				ethFrame[5]=0x01;	
				configInterfacesConfiguracionSet(sensorStoringAddress,&_sensorStoringCfg[0]);							
			}
			else
			{
				ethFrame[5]=0x00;						
			}			
			ethSendFrame(0,&ethFrame[0],6,&_sensorStoringIp[0], 2234);						
			_sensorStoringStatus=DONE;									
		}	
	}
	
	if (_sensorStoringSetSetpoint==IN_PROGRESS)
	{
		if (_cmd_START_CONFIG==NOT_SENDING)			send_cmd_START_CONFIG(((sensorStoringAddress<<1)+1));
		if (_cmd_DTR==NOT_SENDING)							send_cmd_DTR(_sensorStoringSetSetpointLevel);
		if (_cmd_SET_SETPOINT==NOT_SENDING)			send_cmd_SET_SETPOINT(((sensorStoringAddress<<1)+1));		
		if (_cmd_FINISH_CONFIG==SENT)
		{
			_sensorStoringSetSetpoint=DONE;			
		}			
	}
	
	//if (_sensorReboot==IN_PROGRESS)
	//{
	//	if (_cmd_START_CONFIG==NOT_SENDING)			send_cmd_START_CONFIG(((_sensorStoringAddress<<1)+1));
	//	if (_cmd_START_CONFIG==SENT)						send_cmd_REBOOT(((_sensorStoringAddress<<1)+1));
	//	if (_cmd_REBOOT==SENT)		
	//	{
	//		_sensorReboot=DONE;			
	//	}			
	//}
	
	
	
	if (_sensorStoringQuerySetpoint==IN_PROGRESS)
	{		
		
		if (_cmd_QUERY_SETPOINT==NOT_SENDING)	send_cmd_QUERY_SETPOINT(((sensorStoringAddress<<1)+1));
		if (_cmd_QUERY_SETPOINT==SENT)	
		{	
			if (_cmd_QUERY_SETPOINT_valid==0x01)
			{
					unsigned char ethFrame[5];	
					ethFrame[0]=0x83;								//	FrameType
					ethFrame[1]=0x77;								//	Cmd High
					ethFrame[2]=0xBB;								//	Cmd Low
					ethFrame[3]=0;													//	Lenght=4
					ethFrame[4]=_cmd_QUERY_SETPOINT_answer;	//		Va a iniciar ó está haciendo la operación / No va a iniciar o no está haciendo la operación.
					ethSendFrame(0,&ethFrame[0],5,&_sensorStoringQuerySetpointIp[0], 2234);									
			}		
			_sensorStoringQuerySetpoint=DONE;
		}
	}
	
	
}
