/*

	 H I S T O R I C O T E S T . C
	 
	 14/02/2019.
	 
	 DIEGO LUIS FERNÁNDEZ HACES @ NORMAGRUP TECHNOLOGY.

*/

#include "utils.h"
#include "dali.h"
#include "daliLoop.h"
#include "historico.h"
#include "rtcc.h"
#include "macros.h"
#include "actualLevel.h"

#define DEST_TIPO_NINGUNO						0x00
#define DEST_TIPO_INDIVIDUAL				0x01
#define DEST_TIPO_GRUPO							0x02
#define DEST_TIPO_BC								0x03

unsigned char 	_historicoTest_FtDoneArray[8];
unsigned char 	_historicoTest_FtDoneClearPendingArray[8];
unsigned char 	_historicoTest_DtDoneArray[8];
unsigned char 	_historicoTest_DtDoneClearPendingArray[8];

void 						historicoTest_PeticionManual 					(unsigned char dest, unsigned char event)
{	
	unsigned char destTipo;	
	unsigned char n;
	unsigned char grupo;	
	destTipo=actualLevelDestTipoGet(dest);	
	Date date;
	Time time;
	date=rtccDateGet();
	time=rtccTimeGet();			
	if (destTipo==DEST_TIPO_INDIVIDUAL) 												
	{
		historicoAdd (0x00, ((dest&0x7E)>>1), date.day, date.month, date.year, time.hours, time.minutes, event);						
	}
	if (destTipo==DEST_TIPO_GRUPO)	
	{
		grupo=actualLevelGrupoGet(dest);
		for (n=0;n<64;n++)
		{				
			if (actualLevelAddressPerteneceGrupoGet(n,grupo)==0x01)		
			{
				historicoAdd (0x00, n, date.day, date.month, date.year, time.hours, time.minutes, event);					
			}
		}
	}
	if (destTipo==DEST_TIPO_BC)	
	{	
		for (n=0;n<64;n++)
		{					
			 if ((daliLoopInstalledGet(n)==0x01) && (daliLoopDeviceTypeGet(n)==0x01))		historicoAdd (0x00, n, date.day, date.month, date.year, time.hours, time.minutes, event);								
		}
	}	
}
void 						historicoTest_ProcesaEstaEmergencia		(unsigned char address, unsigned char value)
{		
	Date date;
	Time time;
	date=rtccDateGet();
	time=rtccTimeGet();		
	if (BITVAL(value,1)==0x01)
	{
		utils_BitFromByteArraySet(&_historicoTest_FtDoneArray[0],address);
		utils_BitFromByteArraySet(&_historicoTest_FtDoneClearPendingArray[0],address);
	}
	else																										
	{
		if (utils_BitFromByteArrayGet(&_historicoTest_FtDoneArray[0],address)==0x01)
		{		
			historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes,  HISTORICO_REALIZADO_FT);						
			utils_BitFromByteArrayClear(&_historicoTest_FtDoneArray[0],address);
		}						
		utils_BitFromByteArrayClear(&_historicoTest_FtDoneClearPendingArray[0],address);
	}	

	if (BITVAL(value,2)==0x01)
	{
		utils_BitFromByteArraySet(&_historicoTest_DtDoneArray[0],address);
		utils_BitFromByteArraySet(&_historicoTest_DtDoneClearPendingArray[0],address);
	}
	else																										
	{
		if (utils_BitFromByteArrayGet(&_historicoTest_DtDoneArray[0],address)==0x01)
		{		
			historicoAdd (0x00, address, date.day, date.month, date.year, time.hours, time.minutes,  HISTORICO_REALIZADO_DT);							
			utils_BitFromByteArrayClear(&_historicoTest_DtDoneArray[0],address);
		}						
		utils_BitFromByteArrayClear(&_historicoTest_DtDoneClearPendingArray[0],address);
	}
}
unsigned char 	historicoTest_Send 										(void)
{
	unsigned char n;	
	if ((_historicoTest_FtDoneClearPendingArray[0]!=0x00) || (_historicoTest_FtDoneClearPendingArray[1]!=0x00) || (_historicoTest_FtDoneClearPendingArray[2]!=0x00) || (_historicoTest_FtDoneClearPendingArray[3]!=0x00) || (_historicoTest_FtDoneClearPendingArray[4]!=0x00) || (_historicoTest_FtDoneClearPendingArray[5]!=0x00) || (_historicoTest_FtDoneClearPendingArray[6]!=0x00) || (_historicoTest_FtDoneClearPendingArray[7]!=0x00))
	{			
		if (daliSend(DALI_TEST, 0xFFFF, 0x07, 0xFF, &_historicoTest_FtDoneClearPendingArray[0], 0x2C, 0xE6)==0x01)
		{						
			for (n=0;n<8;n++)	_historicoTest_FtDoneClearPendingArray[n]=0x00;							
		}		
		return 0x00;
		}		
	if ((_historicoTest_DtDoneClearPendingArray[0]!=0x00) || (_historicoTest_DtDoneClearPendingArray[1]!=0x00) || (_historicoTest_DtDoneClearPendingArray[2]!=0x00) || (_historicoTest_DtDoneClearPendingArray[3]!=0x00) || (_historicoTest_DtDoneClearPendingArray[4]!=0x00) || (_historicoTest_DtDoneClearPendingArray[5]!=0x00) || (_historicoTest_DtDoneClearPendingArray[6]!=0x00) || (_historicoTest_DtDoneClearPendingArray[7]!=0x00))
	{			
		if (daliSend(DALI_TEST, 0xFFFF, 0x07, 0xFF, &_historicoTest_DtDoneClearPendingArray[0], 0x2C, 0xE7)==0x01)
		{						
			for (n=0;n<8;n++)	_historicoTest_DtDoneClearPendingArray[n]=0x00;							
		}		
		return 0x00;
	}	
	return 0x01;	
}