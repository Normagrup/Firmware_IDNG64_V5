#include "dali.h"
#include "daliLoop.h"
#include "ethFramming.h"



unsigned char _dir;
unsigned char _lvl;
unsigned char inc;

unsigned char _dirLongPress;


unsigned char _presencySentFlag;
unsigned char _vacancySentFlag;

unsigned char _presency;
unsigned char _vacancy;

unsigned char _recallMaxSentFlag;
unsigned char _offSentFlag;

unsigned char _stopFaddingSentFlag;

unsigned char _longPress;

unsigned char _cmd[16][3];




// Puedo meterlas todas en el mismo registro uint8_t, pero de momento lo hago así.
unsigned char  	_sensorsShortPressPending[64];						// Indica que hay pendiente una acción shortPress
unsigned char 	_sensorsLongPressPressPending[64];				// Indica que hay pendiente una acción longPress Pulsado
unsigned char 	_sensorsLongPressInProgressPending[64];		// Indica que hay pendiente una acción longPress Se ha iniciado
unsigned char 	_sensorsLongPressReleasePending[64];			// Indica que hay pendiente una acción longPress Se ha soltado







void lvlSet (unsigned char lvl)
{
	_lvl=lvl;
}


void sensorsRequestToSend(unsigned char sensorAddress)
{
	unsigned char _apply[8];	
	unsigned char dest;
	dest=(sensorAddress<<1)+1;	
	daliSend(DALI_SENSORS, 0x01, 0x16, dest, &_apply[0], 0x2E, 0xEE);	
}


static unsigned char dest2addressGet (unsigned char dest)
{
	return ((dest&0x7E)>>1);	
}




static unsigned char group2destGet (unsigned char group)
{
	return ((group<<1)+0x81);
}




static unsigned char sensorsShortPressPendingGet (unsigned char n)
{
	return _sensorsShortPressPending[n];
}


static void sensorsShortPressPendingSet (unsigned char n)
{
	_sensorsShortPressPending[n]=0x01;
}


static void sensorsShortPressPendingClear (unsigned char n)
{
	_sensorsShortPressPending[n]=0x00;
}












static unsigned char sensorsLongPressPressPendingGet (unsigned char n)
{
	return _sensorsLongPressPressPending[n];
}

static void sensorsLongPressPressPendingSet (unsigned char n)
{
	_sensorsLongPressPressPending[n]=1;	
}

static void sensorsLongPressPressPendingClear (unsigned char n)
{
	

	_sensorsLongPressPressPending[n]=0;
}


static unsigned char sensorsLongPressReleasePendingGet (unsigned char n)
{
	return _sensorsLongPressReleasePending[n];
}

static void sensorsLongPressReleasePendingSet (unsigned char n)
{
	_sensorsLongPressReleasePending[n]=1;
}

static void sensorsLongPressReleasePendingClear (unsigned char n)
{
	_sensorsLongPressReleasePending[n]=0;
}




static unsigned char sensorsLongPressInProgressPendingGet (unsigned char n)
{
	return _sensorsLongPressInProgressPending[n];
}

static void sensorsLongPressInProgressPendingSet (unsigned char n)
{
	_sensorsLongPressInProgressPending[n]=1;
}

static void sensorsLongPressInProgressPendingClear (unsigned char n)
{
	_sensorsLongPressInProgressPending[n]=0;
}



// Devuelve 0 o 1 en función de lo que deben hacer los equipos (encenderse o apagarse).
static unsigned char sensorsShortPressActionGet (unsigned char group)
{	
	if (daliLoopGroupActualLevelGet(group)==0x01)	// Significa que hay algún equipo apagado.
	{
			return 0x01;	// Enciende
		
	}
	else
	{
			return 0x00;	// Apaga
	}
}










// Devuelve 0 o 1 en función de si un determinado sensor está instalado o no.
static unsigned char sensorInstalledGet (unsigned char address)
{
	return 0x01;
}





// Devuelve 0 o 1 en función de si la función switch está habilitada o no.
static unsigned char sensorSwitchFunctionEnabledGet(unsigned char address)
{	
	return 0x01;	
}






// Realiza el procesado de la función switch.
static void sensorSwitchFunctionProcess(unsigned char sensorAddress,unsigned char status)
{
	unsigned char _apply[8];	
	unsigned char n;	
	unsigned char lvl;
	unsigned char actualLvl;
	
	// >>>Long Press
	if ((status&16)==16)
	{
			sensorsLongPressPressPendingSet(0);
	}
	else	//No Long Press
	{		
			if (sensorsLongPressInProgressPendingGet(0)==1)
			{
				sensorsLongPressReleasePendingSet(0);			
				sensorsLongPressInProgressPendingClear(0);		
			}		
	}					
	// <<< Long Press
			
			
	// >>> Short Press
	if ((status&8)==8)
	{								
			sensorsShortPressPendingSet(0);								
	}
	// <<< Short Press
	
}




















// Envio del registro de estado del sensor.
static void sensorsStatusRegSend(unsigned char id,unsigned char status)
{
		// Enviaría ID del sensor. Nº Serie o FFFFFFFF al principio, un codigo de qué es lo que está enviando y el registro de estado ó el arco.	
		unsigned char report[16];	
		unsigned char ip1[4]={255,255,255,255};			
		report[0]=0x04;			// F. tipo 4.		
		report[1]=0x88;			// Com High Sensor Report		
		report[2]=0x00;			// Com Low	Sensor Status Report				
		report[3]=11;				// Lenght		
		report[4]=id;				// 0  - Id del sensor.
		report[5]=0xFF;			// 1  - YY
		report[6]=0xFF;			// 2  - YY
		report[7]=0xFF;			// 3  - MM
		report[8]=0xFF;			// 4  - 0T
		report[9]=0xFF;			// 5  - TT
		report[10]=0xFF;		// 6  - TT
		report[11]=0xFF;		// 7  - 0N
		report[12]=0xFF;		// 8  - NN
		report[13]=0xFF;		// 9  - NN
		report[14]=0;				// 10 - COD 0
		report[15]=status;	// 11 - STATUS		
		ethSendFrame(0,&report[0],16,&ip1[0], 2234);								
}	













// Se realiza el procesado del registro de estado.

static void sensorsStatusRegProcess (unsigned char dali, unsigned char dest, unsigned char status)
{
	unsigned char sensorAddress;	
	sensorAddress=dest2addressGet(dest);
	if (sensorInstalledGet(sensorAddress)==0x01)
	{		
		if (sensorSwitchFunctionEnabledGet(sensorAddress)==0x01)		sensorSwitchFunctionProcess(sensorAddress,status);			
		// if sensorLightChange... Si dali es true y el tema es un cambio de nivel de luz, se pide el nivel.
		sensorsStatusRegSend(dest,status);	//	Si procede se envia un report del suceso.
		// Realiza el envio del registro de estado del sensor enviando su identificador, numero de serie, codigo de operacion y en este caso el registro de estado.
	}
}




















void sensorsDoTask (void)
{
	unsigned char _apply[8];	
	unsigned char n;
	unsigned char actualLvl;	
	unsigned char lvl;	
	unsigned char i;	
	unsigned char destGroup;			// Grupo destino asociado con el pulsador.
	unsigned char group;		
	for (n=0;n<64;n++)	
	{			
		if (sensorInstalledGet(n)==0x01)
		{		
			// >> Procesado ShortPress
			if (sensorsShortPressPendingGet(n)==0x01)
			{			
				group=0;																		// Se obtiene el grupo asociado al sensor, 0 sería el grupo asociado al sensor 0.
				destGroup=group2destGet(group);							// Devuelve 0x81 que es el dest asociado a G0.			
				if (sensorsShortPressActionGet(n)==0x01)		// Debo Encender
				{
					if (daliSend(DALI_SENSORS, 0xFFFF, 0x01, destGroup, &_apply[0], 0x2B, 0x05)==1)
					{						
						daliLoopActualLevelSet(destGroup,254);	// Se ajusta el nivel de luz del grupo al que hemos mandado en realidad podría ser diferente de 254.
						sensorsShortPressPendingClear(n);				// La acción ya se ha ejecutado por tanto no está pendiente.
					}			
				}
				else
				{																						// Debo Apagar
					if (daliSend(DALI_SENSORS, 0xFFFF, 0x01, destGroup, &_apply[0], 0x2B, 0x00)==1)
					{						
						daliLoopActualLevelSet(destGroup,0);		// Se ajusta el nivel de luz del grupo al que hemos mandado
						sensorsShortPressPendingClear(n);				// La acción ya se ha ejecutado por tanto no está pendiente.					
					}						
				}			
			}			
			
			// << Procesado ShortPress
			// >> Procesado LongPress
			// 		>> Press
			if (sensorsLongPressPressPendingGet(n)==0x01)
			{							
		// 		<< Press			
			actualLvl=daliLoopActualLevelGet (0);			
			if (sensorsLongPressInProgressPendingGet(n)==0x00)
			{					
				inc=1;				
				if (actualLvl==254)		_dirLongPress=0;
				if (actualLvl<=177)		_dirLongPress=1;
				if ((actualLvl>177) && (actualLvl<254))
				{
					if (_dirLongPress==1)	
					{
						_dirLongPress=0;
					}
					else
					{
						_dirLongPress=1;
					}
				}							
				sensorsLongPressInProgressPendingSet(n);						
			}
			else
			{							
				lvl=actualLvl;				
				if (_dirLongPress==0x01)
				{					
					for (i=0;i<inc;i++)
					{
						if (lvl<254)	lvl++;
						if (lvl<177)	lvl=177;
					}																		
				}
				else
				{
					for (i=0;i<inc;i++)
					{
						if (lvl>177)	lvl--;						
					}										
					}			
						if (inc<254)	inc++;												
						if (inc<254)	inc++;									
						if (daliSend(DALI_SENSORS, 0xFFFF, 0x01, 0xFE, &_apply[0], 0x2B, lvl)==1)
						{
							sensorsLongPressPressPendingClear(n);							
							daliLoopActualLevelSet (0xFF,lvl);								
						}				
					}
			}
			
			
				
			//		>> Release
			if (sensorsLongPressReleasePendingGet(n)==0x01)
			{		
				if (daliSend(DALI_SENSORS, 0xFFFF, 0x01, 0xFE, &_apply[0], 0x2B, 255)==1)	
				{
					sensorsLongPressReleasePendingClear(n);
				}				 
			}
			
		
		
			//		<< Release
			// << Procesado LongPress

		}			
	}		
}


void sensorsDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
	unsigned char _apply[8];	
	unsigned char n;
	unsigned char status;
	unsigned char lvl;
	unsigned char actualLvl;
	
	
	// >>> Recibida Lectura de un sensor.	
		if (daliSentSenderPacketId==0x02)
		{								
			actualLvl=daliLoopActualLevelGet (0);
			
			if (actualLvl>0)												// Actúa únicamente cuando la lámpara está encendida.																			
																							// En modo LUZ+PRESENCIA, se seleccionará en el interfaz de sensores si se desea que genere eventos cuando hay ausencia.
																							// Si se envia un comando posterior en una situación de ausencia, el regu
																							// Si está en modo sólo LUZ el regulador actuará siempre.																
			{
				lvl=daliReplyAnswer;
				daliLoopActualLevelSet(0xFF,lvl);								
				if (lvl<177)		lvl=177;
				// Fijaré un nivel siempre superior o igual al min level del equipo con el min level menor.	
				// La función de ir a un nivel de arco también debería desplazarla a sensorsDoTask.
				daliSend(DALI_SENSORS, 0x03, 0x01, 0xFE, &_apply[0], 0x2B, lvl);	
			}			
		}
	// <<< Recibida Lectura de un sensor.	
		
	// >>> Recibido registro de estado.
		if (daliSentSenderPacketId==0x01)
		{						
			sensorsStatusRegProcess(0x01,daliSentDest,daliReplyAnswer);																								
		}
	// >>> Recibido registro de estado.
		
	
	
	
}